// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "DocoptPrivatePCH.h"
#include "Docopt.h"
#include "DocoptBlueprintAPI.h"


TMap<FString, FDocoptValue> UDocoptBlueprintAPI::ParseArguments(const FString& InDoc, const TArray<FString> InArgv, bool InHelp, const FString& InVersion, bool InOptionsFirst, bool& OutHasMessage, FString& OutMessage)
{
	//convert our doc text to std::string for docopt.
	std::string doc = std::string(TCHAR_TO_UTF8(*InDoc));
	
	//empty argument vector.
	std::vector<std::string> argv;

	for (int i = 0; i < InArgv.Num(); i++)
	{
		//push the argument from the input array to the docopt arg vector.
		argv.push_back(std::string(TCHAR_TO_UTF8(*InArgv[i])));
	}

	try {
		//Now we can actually parse the argument vector with docopt and we get an std::map back.
		std::map<std::string, docopt::value> parsemap = docopt::docopt_parse(doc, argv, InHelp, !InVersion.IsEmpty(), InOptionsFirst);

		//But Unreal wants a TMap<FString, FDocoptValue>.
		TMap<FString, FDocoptValue> uArgMap;

		//So we'll iterate through the original map.
		for (auto& arg : parsemap)
		{
			FString argname(arg.first.c_str());
			FDocoptValue argvalue(arg.second);

			//WORKAROUND: if it's a command or an option, find it in the original argv. If it's there, set the value to bool::true.
			if (argname.StartsWith("<") == false && argname.StartsWith("[") == false) 
			{
				if (InArgv.Contains(argname))
				{
					argvalue = FDocoptValue(docopt::value(true));
				}
			}
			else { //WORKAROUND: positionals that are numbers should be registered as EDocoptKind::Number
				FString argValueText = ValueAsString(argvalue);
				if (argValueText.IsNumeric())
				{
					int32 argNum = FCString::Atoi(*argValueText);
					argvalue = FDocoptValue(docopt::value(argNum));
				}
			}
			uArgMap.Add(argname, argvalue);

		}

		//So Blueprint knows there wasn't an error or docopt output message:
		OutHasMessage = false;

		//Return the new argmap to blueprint.
		return uArgMap;
	} catch (docopt::DocoptExitHelp const&) {
		//Docopt is outputting the usage, so, we should as well. The user made an error.
		OutHasMessage = true;
		OutMessage = InDoc;
		return TMap<FString, FDocoptValue>();
	}
	catch (docopt::DocoptExitVersion const&) {
		//Docopt is handling the version command.
		OutHasMessage = true;
		OutMessage = InVersion;
		return TMap<FString, FDocoptValue>();
	}
	catch (docopt::DocoptLanguageError const& error) {
		//A POSIX syntax error has been found in the doc text. Print to engine console.

		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::Red, TEXT("DOCOPT USAGE STRING PARSER ERROR: ") + FString(error.what()));
		OutHasMessage = true;
		OutMessage = TEXT("Docopt parser error. See your debug console for info.");
		return TMap<FString, FDocoptValue>();
	}
	catch (docopt::DocoptArgumentError const& error) {
		OutHasMessage = true;
		OutMessage = FString(error.what()) + TEXT("\r\n") + InDoc;
		return TMap<FString, FDocoptValue>();
	}

}

