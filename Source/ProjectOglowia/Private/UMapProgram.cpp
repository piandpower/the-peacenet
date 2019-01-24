#include "UMapProgram.h"
#include "USystemContext.h"
#include "UMapNodeContext.h"
#include "PeacenetWorldStateActor.h"
#include "UPeacenetSaveGame.h"

UMapNodeContext* UMapProgram::GetContext(int InEntityID)
{
    for(UMapNodeContext* Context : this->LoadedContexts)
    {
        if(Context->GetNodeID() == InEntityID)
            return Context;
    }
    return nullptr;
}

void UMapProgram::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    const int MAX_NODES_PER_FRAME = 10;

    if(NodeIDsToSpawn.Num() && Window)
    {
        for(int i = 0; i < NodeIDsToSpawn.Num() && i <= MAX_NODES_PER_FRAME; i++)
        {
            int NodeID = NodeIDsToSpawn[0];
            NodeIDsToSpawn.RemoveAt(0);

            FPeacenetIdentity Identity;
            int IdentityIndex;
            bool result = this->GetUserContext()->GetPeacenet()->SaveGame->GetCharacterByID(NodeID, Identity, IdentityIndex);
            check(result);

            if(!result) continue;

            if(Identity.Country != this->GetUserContext()->GetOwningSystem()->GetCharacter().Country)
                continue;

            UMapNodeContext* Context = this->GetContext(NodeID);
            if(!Context)
            {
                Context = NewObject<UMapNodeContext>(this);
                Context->Setup(this, NodeID);
                LoadedContexts.Add(Context);
            }
            this->AddNode(Context);
        }
    }
    else
    {
        if(!SpawnedLinksYet)
        {
            SpawnedLinksYet = true;

            TArray<UMapNodeContext*> DoNotSpawn;
            for(auto LeftLink : this->LoadedContexts)
            {
                DoNotSpawn.Add(LeftLink);

                TArray<int> Adjacents = this->GetUserContext()->GetPeacenet()->SaveGame->GetAdjacents(LeftLink->GetNodeID());

                for(auto RightNode : Adjacents)
                {
                    UMapNodeContext* RightLink = GetContext(RightNode);
                    if(!RightLink) continue;
                    if(!DoNotSpawn.Contains(RightLink))
                    {
                        DoNotSpawn.Add(RightLink);

                        this->ConnectNodes(LeftLink, RightLink);
                    }
                }
            }
        }
    }
    Super::NativeTick(MyGeometry, InDeltaTime);
}

void UMapProgram::NativeConstruct()
{
    this->NodeIDsToSpawn = this->GetUserContext()->GetPeacenet()->SaveGame->PlayerDiscoveredNodes;

    this->ClearNodes();

    Super::NativeConstruct();
}