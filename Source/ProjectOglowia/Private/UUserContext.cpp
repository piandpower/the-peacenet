#include "UUserContext.h"
#include "PeacenetWorldStateActor.h"
#include "URainbowTable.h"
#include "UDesktopWidget.h"
#include "PTerminalWidget.h"
#include "UConsoleContext.h"
#include "CommonUtils.h"
#include "UAddressBookContext.h"
#include "UProgram.h"
#include "UPeacegateFileSystem.h"

FUserInfo UUserContext::GetUserInfo()
{
    return this->GetOwningSystem()->GetUserInfo(this->UserID);
}

void UUserContext::Setup(USystemContext* InOwningSystem, int InUserID)
{
    // Make sure the owning system is valid.
    check(InOwningSystem);

    this->OwningSystem = InOwningSystem;
    this->UserID = InUserID;
}

FString UUserContext::GetHostname()
{
    return this->OwningSystem->GetHostname();
}

FString UUserContext::GetUsername()
{
    return this->OwningSystem->GetUsername(this->UserID);
}

FString UUserContext::GetCharacterName()
{
    return this->OwningSystem->GetCharacter().CharacterName;
}

FString UUserContext::GetHomeDirectory()
{
    return this->OwningSystem->GetUserHomeDirectory(this->UserID);
}

UPeacegateFileSystem* UUserContext::GetFilesystem()
{
    return this->OwningSystem->GetFilesystem(this->UserID);
}

URainbowTable* UUserContext::GetRainbowTable()
{
    return this->OwningSystem->GetRainbowTable();
}

UAddressBookContext* UUserContext::GetAddressBook()
{
    return this->OwningSystem->GetAddressBook();
}

APeacenetWorldStateActor* UUserContext::GetPeacenet()
{
    return this->OwningSystem->GetPeacenet();
}

UDesktopWidget* UUserContext::GetDesktop()
{
    // TODO: User context should own the desktop, not the system context. This will allow remote desktop hacking.
    return this->OwningSystem->GetDesktop();
}

USystemContext* UUserContext::GetOwningSystem()
{
    return this->OwningSystem;
}

void UUserContext::LogEvent(FString InEvent)
{
	if(!InEvent.TrimStartAndEnd().IsEmpty())
	{
		this->GetOwningSystem()->LogEvent(this->UserID, InEvent);
	}
}

bool UUserContext::IsAdministrator()
{
	return this->GetUserInfo().IsAdminUser;
}

bool UUserContext::OpenProgram(FName InExecutableName, UProgram*& OutProgram, bool InCheckForExistingWindow)
{
    return this->GetOwningSystem()->OpenProgram(InExecutableName, OutProgram, InCheckForExistingWindow);
}

void UUserContext::ShowProgramOnWorkspace(UProgram* InProgram)
{
    // Check the program and desktop.
    check(InProgram);
    check(this->GetDesktop());

    // Show it on our workspace.
    this->GetDesktop()->ShowProgramOnWorkspace(InProgram);
}

UConsoleContext* UUserContext::CreateConsole(UPTerminalWidget* InTerminalWidget)
{
    // CHeck the terminal widget, our system context, etc
    check(this->GetOwningSystem());
    check(InTerminalWidget);

	UConsoleContext* SubConsole = NewObject<UConsoleContext>(this);

	// Assign it to the terminal widget.
	SubConsole->Terminal = InTerminalWidget;

    // TODO: Give the console context ourselves as a user context,
    // NOT OUR UNDERLYING SYSTEM AND UID.

	// User ID matches our window.
	SubConsole->UserID = this->UserID;
	SubConsole->SystemContext = this->GetOwningSystem();

    // TODO: See above. We have our GetFilesystem() function which the console context should also have.
	SubConsole->Filesystem = this->GetFilesystem();

	// Get user info.
	FUserInfo User = this->GetUserInfo();

	// If the user's username is root, then we set the home directory to "/root."
	if (User.IsAdminUser)
	{
		SubConsole->HomeDirectory = TEXT("/root");
	}
	else
	{
		SubConsole->HomeDirectory = TEXT("/home/") + User.Username;
	}

	SubConsole->WorkingDirectory = SubConsole->HomeDirectory;
	
	// Attempt to create the user directory if it's not there
	if (!this->GetFilesystem()->DirectoryExists(SubConsole->WorkingDirectory))
	{
		EFilesystemStatusCode StatusCode;

		if (!this->GetFilesystem()->CreateDirectory(SubConsole->WorkingDirectory, StatusCode))
		{
			FText Error = UCommonUtils::GetFriendlyFilesystemStatusCode(StatusCode);
			SubConsole->Write(TEXT("peacegate: user home directory '`8") + SubConsole->WorkingDirectory + TEXT("`1' could not be created: "));
			SubConsole->WriteLine(Error.ToString());

			// Working directory becomes /.
			SubConsole->WorkingDirectory = TEXT("/");
		}
	}

	return SubConsole;

}

bool UUserContext::OpenFile(const FString& InPath, EFileOpenResult& OutResult)
{
	if (!this->GetFilesystem())
	{
		OutResult = EFileOpenResult::PermissionDenied;
		return false;
	}

	if (!this->GetFilesystem()->FileExists(InPath))
	{
		OutResult = EFileOpenResult::FileNotFound;
		return false;
	}

	FString Path;
	FString Extension;
	if (!InPath.Split(TEXT("."), &Path, &Extension, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
	{
		OutResult = EFileOpenResult::NoSuitableProgram;
		return false;
	}

	UPeacegateProgramAsset* ProgramAsset;
	if (!this->GetOwningSystem()->GetSuitableProgramForFileExtension(Extension, ProgramAsset))
	{
		OutResult = EFileOpenResult::NoSuitableProgram;
		return false;
	}

    // TODO: Shouldn't the CreateProgram function deal with this internally?
	TSubclassOf<UWindow> WindowClass = this->GetPeacenet()->WindowClass;

	UWindow* NewWindow;
	UProgram* NewProgram = UProgram::CreateProgram(WindowClass, ProgramAsset->ProgramClass, this->GetOwningSystem(), this->UserID, NewWindow);

	NewWindow->WindowTitle = ProgramAsset->AppLauncherItem.Name;
	NewWindow->Icon = ProgramAsset->AppLauncherItem.Icon;
	NewWindow->EnableMinimizeAndMaximize = ProgramAsset->AppLauncherItem.EnableMinimizeAndMaximize;

	NewProgram->FileOpened(InPath);

	return true;
}