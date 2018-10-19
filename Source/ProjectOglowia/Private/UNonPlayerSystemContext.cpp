// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UNonPlayerSystemContext.h"
#include "CommonUtils.h"

FString UNonPlayerSystemContext::GetHostname_Implementation() const
{
	return Computer.Hostname.ToString();
}

UPeacegateFileSystem * UNonPlayerSystemContext::GetFilesystem_Implementation(const int UserID)
{
	TScriptInterface<IFolderRepository> FolderRepo = TScriptInterface<IFolderRepository>(this);

	return UCommonUtils::CreateFilesystem(FolderRepo, UserID);
}

TScriptInterface<ICommandSeeker> UNonPlayerSystemContext::GetCommandSeeker_Implementation()
{
	return TScriptInterface<ICommandSeeker>();
}

FUserInfo UNonPlayerSystemContext::GetUserInfo_Implementation(const int InUserID)
{
	for (FUser User : Computer.Users)
	{
		if (User.ID == InUserID)
		{
			FUserInfo Info;
			Info.Username = User.Username.ToString();
			Info.IsAdminUser = (User.Domain == EUserDomain::Administrator);
			return Info;
		}
	}

	return FUserInfo();
}

void UNonPlayerSystemContext::ShowWindowOnWorkspace_Implementation(const UProgram * InProgram)
{
}

EUserDomain UNonPlayerSystemContext::GetUserDomain_Implementation(int InUserID)
{
	for (FUser User : Computer.Users)
	{
		if (User.ID == InUserID)
		{
			return User.Domain;
		}
	}

	return EUserDomain::User;
}

FText UNonPlayerSystemContext::GetUsername_Implementation(int InUserID)
{
	for (FUser User : Computer.Users)
	{
		if (User.ID == InUserID)
		{
			return User.Username;
		}
	}
	return FText();
}

FString UNonPlayerSystemContext::GetUserHomeDirectory_Implementation(int UserID)
{
	for (FUser User : Computer.Users)
	{
		if (User.ID == UserID)
		{
			if (User.Domain == EUserDomain::Administrator)
				return TEXT("/root");
			return TEXT("/home/") + User.Username.ToString();
		}
	}

	return FString();
}

bool UNonPlayerSystemContext::Authenticate_Implementation(const FString & Username, const FString & Password, int & UserID)
{
	for (FUser User : Computer.Users)
	{
		if (User.Username.ToString() == Username && User.Password.ToString() == Password)
		{
			UserID = User.ID;
			return true;
		}
	}

	return false;
}

bool UNonPlayerSystemContext::GetSuitableProgramForFileExtension_Implementation(const FString & InExtension, UPeacegateProgramAsset *& OutProgram)
{
	return false;
}

void UNonPlayerSystemContext::GetFolderTree_Implementation(TArray<FFolder>& OutFolderTree)
{
	OutFolderTree = Computer.Filesystem;
}

void UNonPlayerSystemContext::PushFolderTree_Implementation(const TArray<FFolder>& InFolderTree)
{
	Computer.Filesystem = InFolderTree;
}
