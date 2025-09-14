#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "CharacterStats.generated.h"

USTRUCT(BlueprintType)
struct FCharacterStats:public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Stats")
	FString Name;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Stats")
	float Health;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Stats")
	float Stamina;

	FCharacterStats():Name(TEXT("Default")),Health(100.f),Stamina(100.f){}
};
