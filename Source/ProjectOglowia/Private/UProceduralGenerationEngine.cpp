#include "UProceduralGenerationEngine.h"
#include "UPeacenetSaveGame.h"
#include "PeacenetWorldStateActor.h"

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