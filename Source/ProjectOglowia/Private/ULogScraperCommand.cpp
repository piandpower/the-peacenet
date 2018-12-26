#include "ULogScraperCommand.h"
#include "USystemContext.h"
#include "PeacenetWorldStateActor.h"
#include "UPeacenetSaveGame.h"
#include "URainbowTable.h"
#include "Base64.h"
#include "UCommandProcessor.h"

void ULogScraperCommand::NativeRunCommand(UConsoleContext* InConsole, const TMap<FString, UDocoptValue*> InArguments)
{
    // Check if we're in a unix pipe. If we're not then we simply exit with an error.
    if(!InConsole->IsA<UPiperContext>())
    {
        InConsole->WriteLine("Error: Cannot scrape input stream because no pipe was detected.");
        this->Complete();
        return;
    }

    // The text stored in the previous command's output. Only available when ran in a unix pipe.
    FString InputBuffer = Cast<UPiperContext>(InConsole)->GetInputBuffer();

    // This is the current save game because this is a C++ command and we can do that.
    UPeacenetSaveGame* SaveGame = InConsole->SystemContext->GetPeacenet()->SaveGame;

    // The list of scraped passwords.
    TArray<FString> FoundPasswords;

    // For every password in the save game's master password table...
    for(auto& MPTEntry : SaveGame->MPT)
    {
        // This is the plain text password.
        FString Password;

        // Decode the base64 string in the MPT.
        FBase64::Decode(MPTEntry, Password);

        // If the input buffer contains the password and we haven't scraped it out yet...
        if(InputBuffer.Contains(Password) && !FoundPasswords.Contains(Password))
        {
            // Scrape it!
            FoundPasswords.Add(Password);
        }
    }

    // For every password that we scraped above, we print it out and add to the rainbow table.
    for(auto& Password : FoundPasswords)
    {
        InConsole->WriteLine("Scraped password: " + Password);
        InConsole->SystemContext->GetRainbowTable()->AddPassword(Password);
    }

    // If no passwords were found, the player is alerted.
    if(!FoundPasswords.Num())
    {
        InConsole->WriteLine("No passwords scraped.");
    }

    // mark the command as complete so we return to the caller.
    this->Complete();
}