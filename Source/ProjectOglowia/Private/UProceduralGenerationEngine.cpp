#include "UProceduralGenerationEngine.h"
#include "UPeacenetSaveGame.h"
#include "PeacenetWorldStateActor.h"

void UProceduralGenerationEngine::Initialize(APeacenetWorldStateActor* InPeacenet)
{
    check(!this->Peacenet);
    check(InPeacenet);

    this->Peacenet = InPeacenet;
}