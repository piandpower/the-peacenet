// Copyright (c) 2018 The Peacenet & Alkaline Thunder.


#include "UPattern.h"
#include "Regex.h"
#include "UDocoptValue.h"

static TArray<FString> Split(FString x) 
{
    checkNoEntry();
    return TArray<FString>();
}

PatternList ULeafPattern::Flatten(bool(*filter)(UPattern const*))
{
    if(filter(this))
    {
        PatternList ret;
        ret.Add(this);
        return ret;
    }

    return PatternList();
}

void ULeafPattern::CollectLeaves(TArray<ULeafPattern*>& OutLeaves)
{
    OutLeaves.Add(this);
}

bool ULeafPattern::HasValue() const
{
    if(!this->Value)
    {
        return false;
    }

    return this->Value->IsValueValid();
}

UDocoptValue* ULeafPattern::GetValue() const
{
    return this->Value;
}

void ULeafPattern::SetValue(UDocoptValue* InValue)
{
    this->Value = InValue;
}

FString const& ULeafPattern::Name() const
{
    return this->LeafName;
}

int ULeafPattern::HashPattern() const
{
    TArray<TCHAR> NameChars = this->LeafName.GetCharArray();
    int NameHash = FCrc::MemCrc32(NameChars.GetData(), NameChars.Num() * sizeof(TCHAR));
    int ValueHash = 0;
    if(this->Value)
    {
        ValueHash = this->Value->HashValueData();
    }

    TArray<int> Combined;
    Combined.Add(NameHash);
    Combined.Add(ValueHash);

    return FCrc::MemCrc32(Combined.GetData(), Combined.Num() * sizeof(int));
}

UPattern* UBranchPattern::Fix()
{
    UniquePatternSet PatternSet;
    this->FixIdentities(PatternSet);
    this->FixRepeatingArguments();
    return this;
}

FString const& UBranchPattern::Name() const
{
    checkNoEntry(); // This should never be called.
    return EmptyName;
}

UDocoptValue* const UBranchPattern::GetValue() const
{
    checkNoEntry(); // should also never be called.
    return nullptr;
}

PatternList UBranchPattern::Flatten(bool(*filter)(UPattern const*))
{
    PatternList ret;
    if(filter(this))
    {
        ret.Add(this);
        return ret;
    }

    for(auto& Child : Children)
    {
        PatternList SubList = Child->Flatten(filter);
        ret.Append(SubList);
    }

    return ret;
}

void UBranchPattern::CollectLeaves(TArray<ULeafPattern*>& OutLeaves)
{
    for(auto& Child : Children)
    {
        Child->CollectLeaves(OutLeaves);
    }
}

void UBranchPattern::SetChildren(PatternList InChildren)
{
    this->Children = InChildren;
}

void UBranchPattern::FixIdentities(UniquePatternSet& InPatterns)
{
    for(auto& Child : Children)
    {
        UBranchPattern* Branch = Cast<UBranchPattern>(Child);
        if(Branch)
        {
            Branch->FixIdentities(InPatterns);
        }

        InPatterns.AddUnique(Child);
    }
}

int UBranchPattern::HashPattern() const
{
    TArray<int> HashParts;
    HashParts.Add(Children.Num());
    for(auto Child : Children)
    {
        HashParts.Add(Child->HashPattern());
    }
    return FCrc::MemCrc32(HashParts.GetData(), sizeof(int) * HashParts.Num());
}

TArray<ULeafPattern*> UPattern::Leaves()
{
    TArray<ULeafPattern*> Ret;
    this->CollectLeaves(Ret);
    return Ret;
}

static bool FindBranchPattern(PatternList InPatternList, UBranchPattern*& OutBranch)
{
    for(auto Child : InPatternList)
    {
        UBranchPattern* Branch = Cast<UBranchPattern>(Child);
        if(Branch)
        {
            OutBranch = Branch;
            return true;
        }
    }
    return false;
}

static TArray<PatternList> Transform(PatternList Pattern)
{
    TArray<PatternList> Result;

    TArray<PatternList> Groups;
    Groups.Add(Pattern);

    while(Groups.Num() > 0)
    {
        PatternList Children = Groups[0];
        Groups.RemoveAt(0);

        UBranchPattern* branch = nullptr;
        if(!FindBranchPattern(Children, branch))
        {
            Result.Add(Children);
            continue;
        }

        Children.Remove(branch);

        if(UEither* either = Cast<UEither>(branch))
        {
            for(auto& EitherChild : either->Children)
            {
                PatternList Group;
                Group.Add(EitherChild);
                Groups.Add(Group);
            }
        }
        else if(UOneOrMore* oneOrMore = Cast<UOneOrMore>(branch))
        {
            PatternList subChildren = oneOrMore->Children;
            subChildren.Append(oneOrMore->Children);
            subChildren.Append(Children);
            Groups.Add(subChildren);
        }
        else
        {
            PatternList Group = branch->Children;
            Group.Append(Children);

            Groups.Add(Group);
        }
    }

    return Result;
}

static int CountElements(PatternList& InList, UPattern* InPattern)
{
    int count = 0;
    for(auto& e : InList)
    {
        if(e->HashPattern() == InPattern->HashPattern())
            count++;
    }
    return count;
}

void UBranchPattern::FixRepeatingArguments()
{
    TArray<PatternList> Either = Transform(this->Children);
    for(auto& group : Either)
    {
        for(auto& e : group)
        {
            if(CountElements(group, e) == 1)
            {
                continue;
            }

            ULeafPattern* leaf = Cast<ULeafPattern>(e);
            if(!leaf) continue;

            bool ensureList = false;
            bool ensureInt = false;

            if(Cast<UCommand>(leaf))
            {
                ensureInt = true;
            }
            else if(Cast<UArgument>(leaf))
            {
                ensureList = true;
            }
            else if(UOption* option = Cast<UOption>(leaf))
            {
                if(option->ArgumentCount)
                {
                    ensureList = true;
                }
                else
                {
                    ensureInt = true;
                }
            }

            if(ensureList)
            {
                TArray<FString> newValue;
                if(leaf->Value->IsString())
                {
                    newValue = Split(leaf->Value->AsString());
                }
                if(!leaf->Value->IsList())
                {
                    leaf->Value->SetValue(newValue);
                }
            }
            else if(ensureInt)
            {
                leaf->Value->SetValue(0);
            }
        }
    }
}

bool ULeafPattern::Match(PatternList& InLeft, TArray<ULeafPattern*>& Collected) const
{
    auto match = SingleMatch(InLeft);
    if(!match.Second)
    {
        return false;
    }

    InLeft.RemoveAt(match.First);

    ULeafPattern* SameName;
    for(auto Leaf :  Collected)
    {
        if(Leaf->Name() == this->Name())
        {
            SameName = Leaf;
            break;
        }
    }

    if(Value->IsNumber())
    {
        int val = 1;
        if(!SameName)
        {
            Collected.Add(match.Second);
            match.Second->SetValue(val);
        }
        else if(SameName->Value->IsNumber())
        {
            val += SameName->Value->AsNumber();
            SameName->SetValue(val);
        }
        else
        {
            SameName->SetValue(val);
        }
    }
    else if(Value->IsList())
    {
        TArray<FString> val;
        if(match.Second->Value->IsString())
        {
            val.Add(match.Second->Value->AsString());
        }
        else if(match.Second->Value->IsList())
        {
            val = match.Second->Value->AsList();
        }

        if(!SameName)
        {
            Collected.Add(match.Second);
            match.Second->SetValue(val);
        }
        else if(SameName->Value->IsList())
        {
            TArray<FString> list = SameName->Value->AsList();
            val.Append(list);
            SameName->SetValue(val);
        }
        else
        {
            SameName->SetValue(val);
        }
    }
    else
    {
        Collected.Add(match.Second);
    }

    return true;
}

FPair<int, ULeafPattern*> UArgument::SingleMatch(PatternList const& left) const
{
    FPair<int, ULeafPattern*> ret;

    for(int i = 0; i < left.Num(); i++)
    {
        UArgument* arg = Cast<UArgument>(left[i]);

        if(arg)
        {
            ret.First = i;
            ret.Second = arg;
            break;
        }
    }

    return ret;
}

FPair<int, ULeafPattern*> UCommand::SingleMatch(PatternList const& left) const
{
    FPair<int, ULeafPattern*> ret;
    for(int i = 0; i < left.Num(); i++)
    {
        UArgument* arg = Cast<UArgument>(left[i]);

        if(arg)
        {
            if(!arg->Value->IsString())
                continue;

            if(Name() == arg->Value->AsString())
            {
                ret.First = i;
                ret.Second = arg;
            }
            break;
        }
    }

    return ret;
}

UOption* UOption::Parse(FString const& InOptionDescription)
{
    FString shortOpt, longOpt;
    int argcount = 0;
    UDocoptValue* val = NewObject<UDocoptValue>();

    auto doubleSpace = InOptionDescription.Find(TEXT("  "));
    auto lastIndex = InOptionDescription.GetCharArray().Num() - 1;
    if(doubleSpace != -1)
    {
        lastIndex = doubleSpace;
    }

    FRegexPattern RegexPattern(TEXT("(-{1,2})?(.*?)([,= ]|$)"));
    FRegexMatcher Matcher(RegexPattern, InOptionDescription);
    Matcher.SetLimits(0, lastIndex);
    while(Matcher.FindNext())
    {
        FString Dashes = Matcher.GetCaptureGroup(1);
        FString Name = Matcher.GetCaptureGroup(2);
        if(!Dashes.IsEmpty())
        {
            if(Dashes == TEXT("-"))
            {
                shortOpt = Dashes + Name;
            }
            else if(Dashes == TEXT("--"))
            {
                longOpt = Dashes + Name;
            }
        }
        else if(!Name.IsEmpty())
        {
            argcount = 1;
        }

        if(!Matcher.GetCaptureGroup(3).IsEmpty())
        {
            break;
        }
    }
    RegexPattern = FRegexPattern(TEXT("\\[default: (.*)\\]"));
    Matcher = FRegexMatcher(RegexPattern, InOptionDescription);
    Matcher.SetLimits(lastIndex, InOptionDescription.GetCharArray().Num()-1);

    if(Matcher.FindNext())
    {
        FString Matched = Matcher.GetCaptureGroup(1);
        val->SetValue(Matched);
    }

    UOption* opt = NewObject<UOption>();

    opt->ShortOption = shortOpt;
    opt->LongOption = longOpt;
    opt->ArgumentCount = argcount;
    opt->Value = val;

    return opt;
}

FPair<int, ULeafPattern*> UOption::SingleMatch(PatternList const& left) const
{
    ULeafPattern* leaf;
    for(int i = 0; i < left.Num(); i++)
    {
        leaf = Cast<ULeafPattern>(left[i]);
        if(leaf && leaf->Name() == Name())
        {
            FPair<int, ULeafPattern*> ret;
            ret.First = i;
            ret.Second = leaf;
            return ret;
        }
    }

    return FPair<int, ULeafPattern*>();
}

bool URequired::Match(PatternList& InLeft, TArray<ULeafPattern*>& Collected) const
{
    auto l = InLeft;
    auto c = Collected;
    for(auto& Child : Children)
    {
        if(!Child->Match(l, c))
        {
            return false;
        }
    }

    InLeft = l;
    Collected = c;
    return true;
}

bool UOneOrMore::Match(PatternList& InLeft, TArray<ULeafPattern*>& Collected) const
{
    check(Children.Num()==1);
    
    PatternList l = InLeft;
	auto c = Collected;

	bool matched = true;
	size_t times = 0;

	PatternList l_;
	bool firstLoop = true;

	while (matched) 
    {
		// could it be that something didn't match but changed l or c?
		matched = Children[0]->Match(l, c);

		if (matched)
			++times;

		if (firstLoop) 
        {
			firstLoop = false;
		} 
        else if (l == l_) 
        {
			break;
		}
		l_ = l;
	}

	if (times == 0) 
    {
		return false;
	}

	InLeft = std::move(l);
	Collected = std::move(c);
	return true;
}

bool UEither::Match(PatternList& InLeft, TArray<ULeafPattern*>& Collected) const
{
    using Outcome = FPair<PatternList, TArray<ULeafPattern*>>;

    TArray<Outcome> outcomes;

    for(auto& child : Children)
    {
        auto l = InLeft;
        auto c = Collected;
        bool matched = child->Match(l,c);
        if(matched)
        {
            outcomes.Add(Outcome(l, c));
        }
    }

    if(outcomes.Num() == 0)
    {
        return false;
    }

    Outcome minOutcome = outcomes[0];

    for(auto o : outcomes)
    {
        if(o.First.Num() < minOutcome.First.Num())
        {
            minOutcome = o;
        }
    }

    InLeft = minOutcome.First;
    Collected = minOutcome.Second;
    return true;
}

void ULeafPattern::SetValue(bool InValue)
{
    UDocoptValue* NewValue = NewObject<UDocoptValue>();
    NewValue->SetValue(InValue);
    this->Value = NewValue;
}

void ULeafPattern::SetValue(int InValue)
{
    UDocoptValue* NewValue = NewObject<UDocoptValue>();
    NewValue->SetValue(InValue);
    this->Value = NewValue;
}

void ULeafPattern::SetValue(FString InValue)
{
    UDocoptValue* NewValue = NewObject<UDocoptValue>();
    NewValue->SetValue(InValue);
    this->Value = NewValue;
}

void ULeafPattern::SetValue(TArray<FString> InValue)
{
    UDocoptValue* NewValue = NewObject<UDocoptValue>();
    NewValue->SetValue(InValue);
    this->Value = NewValue;
}
