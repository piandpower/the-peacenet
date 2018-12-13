// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UHashFunctions.h"
#include "PlatformMisc.h"
#include "SecureHash.h"
#include "picosha2.h"
#include "Base64.h"
#include <string>
#include <vector>
#include <iostream>

using namespace picosha2;

FString UHashFunctions::SHA256Hash(FString InString)
{
	if(InString.IsEmpty())
	{
		return InString;
	}

	std::string src_str(TCHAR_TO_UTF8(InString.GetCharArray().GetData()));

	std::cout << "Hashing string \"" << src_str << "\" using picosha2..." << std::endl;

	std::vector<unsigned char> hash(picosha2::k_digest_size);
	picosha2::hash256(src_str.begin(), src_str.end(), hash.begin(), hash.end());

	std::string hex_str = picosha2::bytes_to_hex_string(hash.begin(), hash.end());

	std::cout << "Done hashing." << std::endl;

	return FString(hex_str.c_str());
}

FString UHashFunctions::MD5Hash(FString InString)
{
	if(InString.IsEmpty())
	{
		return InString;
	}
	return FMD5::HashAnsiString(InString.GetCharArray().GetData());
}

FString UHashFunctions::CrcHash(FString InString)
{
	int Len = InString.GetCharArray().Num();

	auto Hash = FCrc::MemCrc32(InString.GetCharArray().GetData(), sizeof(TCHAR) * Len);

	return FBase64::Encode((uint8*)(&Hash), sizeof(uint32));
}