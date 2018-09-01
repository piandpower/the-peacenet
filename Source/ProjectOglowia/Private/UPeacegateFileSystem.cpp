// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UPeacegateFileSystem.h"

void UPeacegateFileSystem::BuildChildNavigators(UFolderNavigator * RootNav)
{
	FFolder Folder = FolderTree[RootNav->FolderIndex];

	for (auto SubfolderIndex : Folder.SubFolders)
	{
		FFolder SubFolder = FolderTree[SubfolderIndex];
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
			int FolderCount = FolderTree.Num();
			FFolder NewFolder;
			NewFolder.FolderName = Part;
			NewFolder.FolderID = FolderCount;
			NewFolder.ParentID = Navigator->FolderIndex;
			FolderTree.Add(NewFolder);

			FFolder CurrFolder = FolderTree[Navigator->FolderIndex];

			CurrFolder.SubFolders.Add(NewFolder.FolderID);;

			FolderTree[Navigator->FolderIndex] = CurrFolder;

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
	return false;
}

void UPeacegateFileSystem::Delete(const FString InPath)
{
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
	FFolder Folder = FolderTree[Navigator->FolderIndex];

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
