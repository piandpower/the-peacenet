#include "UProceduralGenerationEngine.h"
#include "UPeacenetSaveGame.h"
#include "Base64.h"
#include "UMarkovChain.h"
#include "PeacenetWorldStateActor.h"

FString UProceduralGenerationEngine::GenerateIPAddress(ECountry InCountry)
{
    uint8 Byte1, Byte2, Byte3, Byte4 = 0;

    // First byte is the country range.
    if(this->Peacenet->SaveGame->CountryIPRanges.Contains(InCountry))
    {
        Byte1 = this->Peacenet->SaveGame->CountryIPRanges[InCountry];
    }
    else
    {
        bool taken = false;

        do
        {
            // Generate a new one!
            Byte1 = (uint8)RNG.RandRange(0, 255);
        
            for(auto Elem : this->Peacenet->SaveGame->CountryIPRanges)
            {
                if(Elem.Value == Byte1)
                {
                    taken = true;
                    break;
                }
            }

        } while(taken);

        this->Peacenet->SaveGame->CountryIPRanges.Add(InCountry, Byte1);
    }

    // The other three are easy.
    Byte2 = (uint8)RNG.RandRange(0, 255);
    Byte3 = (uint8)RNG.RandRange(0, 255);
    Byte4 = (uint8)RNG.RandRange(0, 255);
    
    // We only support IPv4 in 2025 lol.
    return FString::FromInt(Byte1) + "." + FString::FromInt(Byte2) + "." + FString::FromInt(Byte3) + "." + FString::FromInt(Byte4);
}

void UProceduralGenerationEngine::Initialize(APeacenetWorldStateActor* InPeacenet)
{
    check(!this->Peacenet);
    check(InPeacenet);

    this->Peacenet = InPeacenet;

    // Initialize the world seed if the game is new.
    if(Peacenet->SaveGame->IsNewGame)
    {
        // Get the player character.
        FPeacenetIdentity Character;
        int CharacterIndex = -1;
        bool result = Peacenet->SaveGame->GetCharacterByID(Peacenet->SaveGame->PlayerCharacterID, Character, CharacterIndex);
        check(result);

        // This creates a hash out of the character name which we can seed the RNG with.
        // Thus making the player's name dictate how the world generates.
        TArray<TCHAR> Chars = Character.CharacterName.GetCharArray();
        int Hash = FCrc::MemCrc32(Chars.GetData(), Chars.Num() * sizeof(TCHAR));

        // Store the seed in the save file in case we need it. WHICH WE FUCKING WILL LET ME TELL YOU.
        Peacenet->SaveGame->WorldSeed = Hash;
    }

    // Recall when we set the world seed in the save file?
    // This is where we need it.
    this->RNG = FRandomStream(this->Peacenet->SaveGame->WorldSeed);
}

FComputer& UProceduralGenerationEngine::GenerateComputer(FString InHostname, EComputerOwnerType InOwnerType)
{
    FComputer Ret;

    // Set up the core metadata.
    Ret.ID = this->Peacenet->SaveGame->Computers.Num();
    Ret.OwnerType = InOwnerType;
    
    // Create the barebones filesystem.
    FFolder Root;
    Root.FolderID = 0;
    Root.FolderName = "";
    Root.ParentID = -1;

    FFolder RootHome;
    RootHome.FolderID = 1;
    RootHome.FolderName = "root";
    RootHome.ParentID = 0;

    FFolder UserHome;
    UserHome.FolderID = 2;
    UserHome.FolderName = "home";
    UserHome.ParentID = 0;

    FFolder Etc;
    Etc.FolderID = 3;
    Etc.FolderName = "etc";
    Etc.ParentID = 0;

    // Write the hostname to a file.
    FFile HostnameFile;
    HostnameFile.FileName = "hostname";
    HostnameFile.FileContent = FBase64::Encode(InHostname);

    // Write the file in /etc.
    Etc.Files.Add(HostnameFile);

    // Link up the three folders to the root.
    Root.SubFolders.Add(RootHome.FolderID);
    Root.SubFolders.Add(Etc.FolderID);
    Root.SubFolders.Add(UserHome.FolderID);
    
    // Add all the folders to the computer's disk.
    Ret.Filesystem.Add(Root);
    Ret.Filesystem.Add(Etc);
    Ret.Filesystem.Add(RootHome);
    Ret.Filesystem.Add(UserHome);
    
    // Add the computer to the save file.
    this->Peacenet->SaveGame->Computers.Add(Ret);

    // Grab the index of that computer in the save.
    int ComputerIndex = this->Peacenet->SaveGame->Computers.Num() - 1;

    // Return it.
    return this->Peacenet->SaveGame->Computers[ComputerIndex];
}