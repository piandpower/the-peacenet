#include "UProgram.h"
#include "UConsoleContext.h"
#include "UWindow.h"
#include "USystemContext.h"
#include "UUserContext.h"
#include "CommonUtils.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "PeacenetWorldStateActor.h"
#include "UPeacegateFileSystem.h"

UUserContext* UProgram::GetUserContext()
{
	return this->Window->GetUserContext();
}

void UProgram::PushNotification(const FText & InNotificationMessage)
{
	this->GetUserContext()->GetDesktop()->EnqueueNotification(this->Window->WindowTitle, InNotificationMessage, this->Window->Icon);
}

void UProgram::RequestPlayerAttention(bool PlaySound)
{
	this->PlayerAttentionNeeded.Broadcast(PlaySound);
}

UProgram* UProgram::CreateProgram(const TSubclassOf<UWindow> InWindow, const TSubclassOf<UProgram> InProgramClass, USystemContext* InSystem, const int InUserID, UWindow*& OutWindow, bool DoContextSetup)
{
	// Preventative: make sure the system context isn't null.
	check(InSystem);

	// TODO: Take in a user context instead of a system context and user ID.
	check(InSystem->GetPeacenet());

	// Grab a user context and check if it's valid.
	UUserContext* User = InSystem->GetUserContext(InUserID);

	check(User);

	APlayerController* MyPlayer = UGameplayStatics::GetPlayerController(InSystem->GetPeacenet()->GetWorld(), 0);

	// The window is what contains the program's UI.
	UWindow* Window = CreateWidget<UWindow, APlayerController>(MyPlayer, InWindow);

	// Construct the actual program.
	UProgram* ProgramInstance = CreateWidget<UProgram, APlayerController>(MyPlayer, InProgramClass);

	// Program and window are friends with each other
	ProgramInstance->Window = Window;

	// Window gets our user context.
	Window->SetUserContext(User);

	// Set up the program's contexts if we're told to.
	if (DoContextSetup)
	{
		ProgramInstance->SetupContexts();
		ProgramInstance->GetUserContext()->ShowProgramOnWorkspace(ProgramInstance);
	}

	// Return the window and program.
	OutWindow = Window;

	return ProgramInstance;
}

void UProgram::ActiveProgramCloseEvent()
{
	if (this->Window->HasAnyUserFocus() || this->Window->HasFocusedDescendants() || this->Window->HasKeyboardFocus())
	{
		this->Window->Close();
	}
}

void UProgram::NativeConstruct()
{
	if (JustOpened)
	{
		TScriptDelegate<> OnActiveProgramClose;
		OnActiveProgramClose.BindUFunction(this, "ActiveProgramCloseEvent");
		this->GetUserContext()->GetDesktop()->EventActiveProgramClose.Add(OnActiveProgramClose);

		JustOpened = false;

		Super::NativeConstruct();
	}
}

void UProgram::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UProgram::ShowInfoWithCallbacks(const FText & InTitle, const FText & InMessage, const EInfoboxIcon InIcon, const EInfoboxButtonLayout ButtonLayout, const bool ShowTextInput, const FInfoboxDismissedEvent & OnDismissed, const FInfoboxInputValidator & ValidatorFunction)
{
	Window->ShowInfoWithCallbacks(InTitle, InMessage, InIcon, ButtonLayout, ShowTextInput, OnDismissed, ValidatorFunction);
}

void UProgram::ShowInfo(const FText & InTitle, const FText & InMessage, const EInfoboxIcon InIcon)
{
	Window->ShowInfo(InTitle, InMessage, InIcon);
}

void UProgram::AskForFile(const FString InBaseDirectory, const FString InFilter, const EFileDialogType InDialogType, const FFileDialogDismissedEvent & OnDismissed)
{
	Window->AskForFile(InBaseDirectory, InFilter, InDialogType, OnDismissed);
}

void UProgram::SetupContexts()
{
	// Add ourself to the window's client slot.
	this->Window->AddWindowToClientSlot(this);

	this->NativeProgramLaunched();
}

void UProgram::SetWindowMinimumSize(FVector2D InSize)
{
	Window->SetClientMinimumSize(InSize);
}

void UProgram::NativeProgramLaunched() {}

