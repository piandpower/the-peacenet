// Copyright (c) 2018 The Peacenet & Alkaline Thunder.


#include "UDocoptValue.h"

bool UDocoptValue::IsValueValid() const
{
    return (this->Kind != EDocoptValueKind::Empty);
}

bool UDocoptValue::IsBoolean() const
{
    return (this->Kind == EDocoptValueKind::Boolean);
}

bool UDocoptValue::IsNumber() const
{
    return (this->Kind == EDocoptValueKind::Number);
}

bool UDocoptValue::IsString() const
{
    return (this->Kind == EDocoptValueKind::String);
}

bool UDocoptValue::IsList() const
{
    return (this->Kind == EDocoptValueKind::List);
}

bool UDocoptValue::AsBoolean() const
{
    check(this->IsBoolean());
    return this->BooleanValue;
}
 
int UDocoptValue::AsNumber() const
{
    check(this->IsNumber());
    return this->NumberValue;
}

FString UDocoptValue::AsString() const
{
    check(this->IsString());
    return this->StringValue;
}

TArray<FString> UDocoptValue::AsList() const
{
    check(this->IsList());
    return this->ListValue;
}

FString UDocoptValue::GetSerializedValue() const
{
    if(IsBoolean())
    {
        return (AsBoolean() ? TEXT("true") : TEXT("false"));
    }
    if(IsNumber())
    {
        return FString::FromInt(this->AsNumber());
    }
    if(IsString())
    {
        return this->AsString();
    }

    if(IsList())
    {
        FString Ret;
        for(FString Val : AsList())
        {
            Ret.Append(Val);
        }
        return Ret;
    }

    return TEXT(":(){:|:};:");
}

int UDocoptValue::HashValueData() const
{
    if(!this->IsValueValid())
        return 0;

    FString Serialized = this->GetSerializedValue();
    TArray<TCHAR> Chars = Serialized.GetCharArray();
    return FCrc::MemCrc32(Chars.GetData(), Chars.Num() * sizeof(TCHAR));
}

bool UDocoptValue::ValueEquals(const UDocoptValue* InOther) const
{
    return (this->HashValueData() == InOther->HashValueData());
}