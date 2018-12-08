// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UHashFunctions.h"
#include "PlatformMisc.h"
#include "SecureHash.h"
#include "Base64.h"

FString UHashFunctions::SHA256Hash(FString InString)
{
	FSHA256Signature OutSig;
	FPlatformMisc::GetSHA256Signature(InString.GetCharArray().GetData(), sizeof(TCHAR) * InString.GetCharArray().Num(), OutSig);
	return OutSig.ToString();
}

FString UHashFunctions::MD5Hash(FString InString)
{
	return FMD5::HashAnsiString(InString.GetCharArray().GetData());
}

FString UHashFunctions::CrcHash(FString InString)
{
	int Len = InString.GetCharArray().Num();

	auto Hash = FCrc::MemCrc32(InString.GetCharArray().GetData(), sizeof(TCHAR) * Len);

	return FBase64::Encode((uint8*)(&Hash), sizeof(uint32));
}