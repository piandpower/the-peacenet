// Copyright (c) 2018 The Peacenet & Alkaline Thunder.

#include "UEmailParser.h"
#include "Json.h"

FString UEmailParser::SerializeEmail(const FEmail InEmail)
{
	FString OutJson = "";

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&OutJson);

	JsonObject->SetStringField("from", InEmail.From.ToString());
	JsonObject->SetStringField("to", InEmail.To.ToString());
	JsonObject->SetStringField("subject", InEmail.Subject.ToString());
	JsonObject->SetStringField("content", InEmail.Content.ToString());

	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	return FString();
}

FEmail UEmailParser::DeserializeEmail(const FString InJson)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(InJson);

	FJsonSerializer::Deserialize(JsonReader, JsonObject);

	FEmail Return;

	Return.From = FText::FromString(JsonObject->GetStringField("from"));
	Return.To = FText::FromString(JsonObject->GetStringField("to"));
	Return.Subject = FText::FromString(JsonObject->GetStringField("subject"));
	Return.Content = FText::FromString(JsonObject->GetStringField("content"));

	return Return;
}