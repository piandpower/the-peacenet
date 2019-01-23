#include "UMapProgram.h"
#include "USystemContext.h"
#include "PeacenetWorldStateActor.h"
#include "UPeacenetSaveGame.h"

void UMapProgram::NativeConstruct()
{
    Super::NativeConstruct();

    this->NodeIDsToSpawn = this->GetUserContext()->GetPeacenet()->SaveGame->GetAllEntitiesInCountry(this->GetUserContext()->GetOwningSystem()->GetCharacter().Country);
}