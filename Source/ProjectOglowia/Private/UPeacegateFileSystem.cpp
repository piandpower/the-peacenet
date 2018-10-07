// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UPeacegateFileSystem.h"

void UPeacegateFileSystem::RecursiveDelete(FFolder& InFolder)
{
	for (auto& Subfolder : InFolder.SubFolders)
	{
		FFolder SubRecord = GetFolderByID(Subfolder);
		RecursiveDelete(SubRecord);
	}
	
	for (int i = 0; i < FolderTree.Num(); i++)
	{
		if (FolderTree[i].FolderID == InFolder.FolderID)
		{
			FolderTree.RemoveAt(i);
			return;
		}
	}
}

FFolder UPeacegateFileSystem::GetFolderByID(int FolderID)
{
	for (auto& Folder : FolderTree)
	{
		if (Folder.FolderID == FolderID)
			return Folder;
	}

	return FFolder();
}

void UPeacegateFileSystem::SetFolderByID(int FolderID, FFolder Folder)
{
	for (int i = 0; i < FolderTree.Num(); i++)
	{
		FFolder ExFolder = FolderTree[i];

		if (ExFolder.FolderID == FolderID)
		{
			FolderTree[i] = Folder;
			return;
		}
	}
}

int UPeacegateFileSystem::GetNewFolderID()
{
	int ID = 0;

	for (auto& Folder : FolderTree)
	{
		if (Folder.FolderID > ID)
		{
			ID = Folder.FolderID;
		}
	}

	return ID+1;
}

void UPeacegateFileSystem::BuildChildNavigators(UFolderNavigator * RootNav)
{
	FFolder Folder = GetFolderByID(RootNav->FolderIndex);

	for (auto SubfolderIndex : Folder.SubFolders)
	{
		FFolder SubFolder = GetFolderByID(SubfolderIndex);
		UFolderNavigator* ChildNav = NewObject<UFolderNavigator>();
		ChildNav->FolderIndex = SubFolder.FolderID;
		RootNav->SubFolders.Add(SubFolder.FolderName, ChildNav);
		BuildChildNavigators(ChildNav);
	}
}

FString UPeacegateFileSystem::ResolveToAbsolute(const FString Path)
{
	TArray<FString> PartStack;
	TArray<FString> Split;

	Path.ParseIntoArray(Split, TEXT("/"), true);

	for (auto& Part : Split)
	{
		if (Part == TEXT("."))
			continue;
		if (Part == TEXT(".."))
		{
			if (PartStack.Num() > 0)
				PartStack.Pop();
			continue;
		}
		PartStack.Push(Part);
	}

	FString Absolute;
	if (PartStack.Num() == 0)
	{
		Absolute = TEXT("/");
	}
	else
	{
		while (PartStack.Num() > 0)
		{
			Absolute = Absolute.Append(TEXT("/") + PartStack[0]);
			PartStack.RemoveAt(0);
		}
	}

	return Absolute;
}

void UPeacegateFileSystem::BuildFolderNavigator()
{
	if (FolderTree.Num()==0)
	{
		UFileUtilities::FormatFilesystem(FolderTree);
		FilesystemModified.Broadcast();
	}

	Root = NewObject<UFolderNavigator>();

	BuildChildNavigators(Root);

}

void UPeacegateFileSystem::CreateDirectory(const FString InPath)
{
	FString ResolvedPath = ResolveToAbsolute(InPath);
	TArray<FString> Parts;
	ResolvedPath.ParseIntoArray(Parts, TEXT("/"), true);
	UFolderNavigator* Navigator = Root;

	bool DirectoryCreated = false;

	for (auto& Part : Parts)
	{
		if (Navigator->SubFolders.Contains(Part))
		{
			Navigator = Navigator->SubFolders[Part];
		}
		else {
			FFolder NewFolder;
			NewFolder.FolderName = Part;
			NewFolder.FolderID = GetNewFolderID();
			NewFolder.ParentID = Navigator->FolderIndex;
			FolderTree.Add(NewFolder);

			FFolder CurrFolder = GetFolderByID(Navigator->FolderIndex);

			CurrFolder.SubFolders.Add(NewFolder.FolderID);;

			SetFolderByID(Navigator->FolderIndex, CurrFolder);

			UFolderNavigator* NewNav = NewObject<UFolderNavigator>();

			NewNav->FolderIndex = NewFolder.FolderID;

			Navigator->SubFolders.Add(NewFolder.FolderName, NewNav);

			Navigator = NewNav;

			DirectoryCreated = true;
		}

	}

	if (DirectoryCreated)
	{
		FilesystemOperation.Broadcast(EFilesystemEventType::CreateDirectory, ResolvedPath);
		FilesystemModified.Broadcast();
	}
}

bool UPeacegateFileSystem::DirectoryExists(const FString InPath)
{
	FString ResolvedPath = ResolveToAbsolute(InPath);
	TArray<FString> Parts;
	ResolvedPath.ParseIntoArray(Parts, TEXT("/"), true);
	UFolderNavigator* Navigator = Root;

	if (Parts.Num() == 0)
		return true;

	for (auto& Part : Parts)
	{
		if (Navigator->SubFolders.Contains(Part))
		{
			Navigator = Navigator->SubFolders[Part];
		}
		else {
			return false;
		}
	}

	return true;
}

bool UPeacegateFileSystem::FileExists(const FString InPath)
{
	FString ResolvedPath = ResolveToAbsolute(InPath);
	TArray<FString> Parts;
	ResolvedPath.ParseIntoArray(Parts, TEXT("/"), true);
	UFolderNavigator* Navigator = Root;

	if (Parts.Num() == 0)
		return false;

	for (int i = 0; i < Parts.Num() - 1; i++)
	{
		auto& Part = Parts[i];

		if (Navigator->SubFolders.Contains(Part))
		{
			Navigator = Navigator->SubFolders[Part];
		}
		else {
			return false;
		}
	}

	FString Filename = Parts[Parts.Num() - 1];

	FFolder FileParent = GetFolderByID(Navigator->FolderIndex);

	for (auto& FileRecord : FileParent.Files)
	{
		if (FileRecord.FileName == Filename)
			return true;

	}

	return false;
}

void UPeacegateFileSystem::Delete(const FString InPath)
{
	if (DirectoryExists(InPath))
	{
		FString ResolvedPath = ResolveToAbsolute(InPath);
		TArray<FString> Parts;
		ResolvedPath.ParseIntoArray(Parts, TEXT("/"), true);
		UFolderNavigator* Navigator = Root;
		
		if (Parts.Num() == 0)
			return;
		
		for (auto& Part : Parts)
		{
			if (Navigator->SubFolders.Contains(Part))
			{
				Navigator = Navigator->SubFolders[Part];
			}
		}

		FFolder FolderToDelete = GetFolderByID(Navigator->FolderIndex);

		int ParentID = FolderToDelete.ParentID;
		int FolderID = FolderToDelete.FolderID;

		RecursiveDelete(FolderToDelete);

		FFolder Parent = GetFolderByID(ParentID);
		Parent.SubFolders.Remove(FolderID);
		SetFolderByID(ParentID, Parent);

		BuildFolderNavigator();

		FilesystemOperation.Broadcast(EFilesystemEventType::DeleteDirectory, ResolvedPath);
		FilesystemModified.Broadcast();

	}
	else if (FileExists(InPath))
	{
		FString FolderPath;
		FString FileName;

		if (!InPath.Split(TEXT("/"), &FolderPath, &FileName, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
			return;

		FString ResolvedPath = ResolveToAbsolute(FolderPath);
		TArray<FString> Parts;
		ResolvedPath.ParseIntoArray(Parts, TEXT("/"), true);
		UFolderNavigator* Navigator = Root;

		for (auto& Part : Parts)
		{
			if (Navigator->SubFolders.Contains(Part))
			{
				Navigator = Navigator->SubFolders[Part];
			}
		}

		FFolder FolderToAlter = GetFolderByID(Navigator->FolderIndex);

		bool Write = false;

		for (int i = 0; i < FolderToAlter.Files.Num(); i++)
		{
			FFile File = FolderToAlter.Files[i];
			if (File.FileName == FileName)
			{
				Write = true;
				FolderToAlter.Files.RemoveAt(i);
				break;
			}
		}

		if (Write)
		{
			SetFolderByID(Navigator->FolderIndex, FolderToAlter);
		
			FilesystemOperation.Broadcast(EFilesystemEventType::DeleteFile, ResolvedPath + TEXT("/") + FileName);
			FilesystemModified.Broadcast();

		}

	}
}

TArray<FString> UPeacegateFileSystem::GetDirectories(const FString & InPath)
{
	FString ResolvedPath = ResolveToAbsolute(InPath);
	TArray<FString> Parts;
	ResolvedPath.ParseIntoArray(Parts, TEXT("/"), true);
	UFolderNavigator* Navigator = Root;

	for (auto& Part : Parts)
	{
		if (Navigator->SubFolders.Contains(Part))
		{
			Navigator = Navigator->SubFolders[Part];
		}
		else {
			return TArray<FString>();
		}
	}

	TArray<FString> Ret;

	TArray<FString> Keys;
	Navigator->SubFolders.GetKeys(Keys);

	for (auto Key : Keys)
	{
		if (ResolvedPath.EndsWith(TEXT("/")))
			Ret.Add(ResolvedPath + Key);
		else
			Ret.Add(ResolvedPath + TEXT("/") + Key);
	}

	return Ret;

}

TArray<FString> UPeacegateFileSystem::GetFiles(const FString & InPath)
{
	FString ResolvedPath = ResolveToAbsolute(InPath);
	TArray<FString> Parts;
	ResolvedPath.ParseIntoArray(Parts, TEXT("/"), true);
	UFolderNavigator* Navigator = Root;

	for (auto& Part : Parts)
	{
		if (Navigator->SubFolders.Contains(Part))
		{
			Navigator = Navigator->SubFolders[Part];
		}
		else {
			return TArray<FString>();
		}
	}

	TArray<FString> Ret;
	FFolder Folder = GetFolderByID(Navigator->FolderIndex);

	for (auto File : Folder.Files)
	{
		FString Key = File.FileName;
		if (ResolvedPath.EndsWith(TEXT("/")))
			Ret.Add(ResolvedPath + Key);
		else
			Ret.Add(ResolvedPath + TEXT("/") + Key);
	}

	return Ret;
}

void UPeacegateFileSystem::WriteText(const FString & InPath, const FString & InText)
{
	if (InPath.EndsWith(TEXT("/")))
		return;

	if (DirectoryExists(InPath))
		return;
	
	FString FolderPath;
	FString FileName;

	if (!InPath.Split(TEXT("/"), &FolderPath, &FileName, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
		return;

	if (!DirectoryExists(FolderPath))
		return;

	FString ResolvedPath = ResolveToAbsolute(FolderPath);
	TArray<FString> Parts;
	ResolvedPath.ParseIntoArray(Parts, TEXT("/"), true);
	UFolderNavigator* Navigator = this->Root;

	for (auto& Part : Parts)
	{
		if (Navigator->SubFolders.Contains(Part))
		{
			Navigator = Navigator->SubFolders[Part];
		}
		else {
			return;
		}
	}

	FFolder Folder = GetFolderByID(Navigator->FolderIndex);

	bool FoundFile = false;

	for (int i = 0; i < Folder.Files.Num(); i++)
	{
		FFile File = Folder.Files[i];

		if (File.FileName == FileName)
		{
			File.FileContent = FBase64::Encode(InText);
			Folder.Files[i] = File;
			FoundFile = true;
			break;
		}
	}

	if (!FoundFile)
	{
		FFile NewFile;
		NewFile.FileName = FileName;
		NewFile.FileContent = FBase64::Encode(InText);
		Folder.Files.Add(NewFile);
	}

	SetFolderByID(Navigator->FolderIndex, Folder);

	FilesystemOperation.Broadcast(EFilesystemEventType::WriteFile, ResolvedPath + TEXT("/") + FileName);
	FilesystemModified.Broadcast();
}

FString UPeacegateFileSystem::ReadText(const FString & InPath)
{
	if (!FileExists(InPath))
		return FString();

	FString FolderPath;
	FString FileName;

	if (!InPath.Split(TEXT("/"), &FolderPath, &FileName, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
		return FString();

	if (!DirectoryExists(FolderPath))
		return FString();

	FString ResolvedPath = ResolveToAbsolute(FolderPath);
	TArray<FString> Parts;
	ResolvedPath.ParseIntoArray(Parts, TEXT("/"), true);
	UFolderNavigator* Navigator = this->Root;

	for (auto& Part : Parts)
	{
		if (Navigator->SubFolders.Contains(Part))
		{
			Navigator = Navigator->SubFolders[Part];
		}
		else {
			return FString();
		}
	}

	FFolder Folder = GetFolderByID(Navigator->FolderIndex);

	for (int i = 0; i < Folder.Files.Num(); i++)
	{
		FFile File = Folder.Files[i];

		if (File.FileName == FileName)
		{
			FString Ret;
			FBase64::Decode(File.FileContent, Ret);
			return Ret;
		}
	}

	return FString();
}

TArray<uint8> UPeacegateFileSystem::ReadBinary(const FString & InPath)
{
	if (!FileExists(InPath))
		return TArray<uint8>();

	FString FolderPath;
	FString FileName;

	if (!InPath.Split(TEXT("/"), &FolderPath, &FileName, ESearchCase::IgnoreCase, ESearchDir::FromEnd))
		return TArray<uint8>();

	if (!DirectoryExists(FolderPath))
		return TArray<uint8>();

	FString ResolvedPath = ResolveToAbsolute(FolderPath);
	TArray<FString> Parts;
	ResolvedPath.ParseIntoArray(Parts, TEXT("/"), true);
	UFolderNavigator* Navigator = this->Root;

	for (auto& Part : Parts)
	{
		if (Navigator->SubFolders.Contains(Part))
		{
			Navigator = Navigator->SubFolders[Part];
		}
		else {
			return TArray<uint8>();
		}
	}

	FFolder Folder = GetFolderByID(Navigator->FolderIndex);

	for (int i = 0; i < Folder.Files.Num(); i++)
	{
		FFile File = Folder.Files[i];

		if (File.FileName == FileName)
		{
			TArray<uint8> Ret;
			FBase64::Decode(File.FileContent, Ret);
			return Ret;
		}
	}

	return TArray<uint8>();
}


bool UPeacegateFileSystem::IsValidAsFileName(const FString & InFileName)
{
	if (InFileName.IsEmpty())
		return false;

	TArray<TCHAR> CharsInString = InFileName.GetCharArray();

	FString AllowedCharString = TEXT("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_- ");

	int index = 0;

	for (auto& Character : CharsInString)
	{
		if (!AllowedCharString.FindChar(Character, index))
			return false;
	}

	return true;
}

bool UPeacegateFileSystem::IsValidAsUserName(const FString & InUserName)
{
	if (InUserName.IsEmpty())
		return false;

	TArray<TCHAR> CharsInString = InUserName.GetCharArray();

	FString AllowedCharString = TEXT("abcdefghijklmnopqrstuvwxyz0123456789_-");

	int index = 0;

	for (auto& Character : CharsInString)
	{
		if (!AllowedCharString.FindChar(Character, index))
			return false;
	}

	return true;
}
