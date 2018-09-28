//Country enum definition.
//Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#pragma once

#include "CoreMinimal.h"
#include "ECountry.generated.h"

UENUM(BlueprintType)
enum class ECountry : uint8
{
	Elytrose,
	Oglowia,
	Sikkim,
	Velacrol,
	Riogan,
	Valkerie,
	Mejionde,

	Num_Countries // Keep this at the bottom, used for procgen. IF YOU ADD SOMETHING BEYOND THIS, THE GAME WILL NOT *EVER* GENERATE NPCS IN THAT COUNTRY. And I'll probably want to swallow you whole for not noticing this very obvious comment telling you NOT TO DO THAT. GOD. KILL ME. THIS COLD IS AWFUL. MY NOSE FEELS LIKE IT'S ABOUT TO EXPLODE INTO A SNOTTY MESS ALL OVER THIS FUCKING CODE.
};
