#include "UMapNodeContext.h"
#include "PeacenetWorldStateActor.h"
#include "UPeacenetSaveGame.h"
#include "UUserContext.h"
#include "UMapProgram.h"

UPeacenetSaveGame* UMapNodeContext::GetSaveGame()
{
    return this->MapProgram->GetUserContext()->GetPeacenet()->SaveGame;
}

int UMapNodeContext::GetSkill()
{
    return this->GetIdentity().Skill;
}

float UMapNodeContext::GetReputation()
{
    return this->GetIdentity().Reputation;
}

FString UMapNodeContext::GetIPAddress()
{
    FComputer Computer;
    int ComputerIndex;
    bool result = this->GetSaveGame()->GetComputerByID(this->GetIdentity().ComputerID, Computer, ComputerIndex);
    check(result);
    return this->MapProgram->GetUserContext()->GetPeacenet()->GetIPAddress(Computer);
}

int UMapNodeContext::GetNodeID()
{
    return this->NodeID;
}

FString UMapNodeContext::CreateBooleanName(FString InExtension)
{
    return "entity." + FString::FromInt(this->NodeID) + ".node." + InExtension;
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