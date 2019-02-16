/********************************************************************************
 * The Peacenet - bit::phoenix("software");
 * 
 * MIT License
 *
 * Copyright (c) 2018-2019 Michael VanOverbeek, Declan Hoare, Ian Clary, 
 * Trey Smith, Richard Moch, Victor Tran and Warren Harris
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * Contributors:
 *  - Michael VanOverbeek <alkaline@bitphoenixsoftware.com>
 *
 ********************************************************************************/


//Country enum definition.

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
