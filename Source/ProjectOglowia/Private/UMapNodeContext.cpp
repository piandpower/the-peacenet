#include "UMapNodeContext.h"
#include "PeacenetWorldStateActor.h"
#include "UPeacenetSaveGame.h"
#include "UUserContext.h"
#include "UMapProgram.h"

UPeacenetSaveGame* UMapNodeContext::GetSaveGame()
{
    return this->MapProgram->GetUserContext()->GetPeacenet()->SaveGame;
}

int UMapNodeContext::GetNodeID()
{
    return this->NodeID;
}

FString UMapNodeContext::GetNodeName()
{
    return this->GetIdentity().CharacterName;
}

FVector2D UMapNodeContext::GetPosition()
{
    FVector2D Ret;
    bool result = this->GetSaveGame()->GetPosition(this->NodeID, Ret);
    check(result);
    return Ret;
}

void UMapNodeContext::Setup(UMapProgram* InMapProgram, int InNodeID)
{
    check(InMapProgram);

    this->MapProgram = InMapProgram;
    this->NodeID = InNodeID;
}

FPeacenetIdentity& UMapNodeContext::GetIdentity()
{
    int IdentityIndex;
    FPeacenetIdentity Identity;
    bool result = this->MapProgram->GetUserContext()->GetPeacenet()->SaveGame->GetCharacterByID(this->NodeID, Identity, IdentityIndex);
    check(result);
    return this->MapProgram->GetUserContext()->GetPeacenet()->SaveGame->Characters[IdentityIndex];
}