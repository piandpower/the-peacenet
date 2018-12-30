#include "UUserContext.h"
#include "PeacenetWorldStateActor.h"
#include "URainbowTable.h"
#include "UDesktopWidget.h"
#include "PTerminalWidget.h"
#include "UConsoleContext.h"
#include "CommonUtils.h"
#include "UVulnerability.h"
#include "UComputerService.h"
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
	SubConsole->SetTerminal(InTerminalWidget);

    // TODO: Give the console context ourselves as a user context,
    // NOT OUR UNDERLYING SYSTEM AND UID.

	// User ID matches our window.
	SubConsole->Setup(this);

	SubConsole->SetWorkingDirectory(SubConsole->GetUserContext()->GetHomeDirectory());
	
	return SubConsole;

}

bool UUserContext::RetrieveUnlockedExploits(UComputerService* InComputerService, TArray<UVulnerability*>& OutVulnerabilities)
{
	auto UnlockedVulns = this->GetOwningSystem()->GetUnlockedVulnerabilities();

	OutVulnerabilities = TArray<UVulnerability*>();

	for(auto Vuln : UnlockedVulns)
	{
		if(Vuln->VulnerableAgainst.Contains(InComputerService))
		{
			OutVulnerabilities.Add(Vuln);
		}
	}

	return OutVulnerabilities.Num();
}

FString UUserContext::GetUserTypeDisplay()
{
	if(this->IsAdministrator())
	{
		return "#";
	}
	else
	{
		return "$";
	}
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