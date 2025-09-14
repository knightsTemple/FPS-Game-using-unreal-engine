// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "SActionComponent.h"

#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

/**
 * 
 */

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()

public:

	FActionRepData()
		: bIsRunning(false), Instigator(nullptr)
	{}
	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	AActor* Instigator;
};

UCLASS(Blueprintable,BlueprintType, Category="Actions")//允许蓝图继承这个类，如果父类或者祖先类已经标注为Blueprintable，则子类无需再次标注。
class TAKANASHI_HOSHINO_API USAction : public UObject
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly,Category="Action")
	FName ActionName;

	UPROPERTY(EditDefaultsOnly,Category="Action")
	bool bAutoStart;
	
	UFUNCTION(BlueprintNativeEvent,Category="Action")
	void StartAction(AActor* Instigator);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent,Category="Action")
	void StopAction(AActor* Instigator);

	UFUNCTION(BlueprintNativeEvent,Category="Action")
	bool CanStart(AActor* Instigator);

	UFUNCTION(BlueprintCallable,Category="Action")
	bool IsRunning()const
	{
		return RepData.bIsRunning;
	}

	UWorld* GetWorld() const override;

	UPROPERTY(ReplicatedUsing="OnRep_RepData")
	FActionRepData RepData;
//	bool bIsRunning;

    UFUNCTION()
	void OnRep_RepData();

	void Initialize(USActionComponent* NewActionComponent);
	
	// ✅ 关键修复：支持网络复制，默认继承AActor不需要重写，会进行网络复制。但是继承UObject时需要重写IsSupportedForNetworking方法。
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
	
protected:

	UPROPERTY(Replicated)
    USActionComponent*ActionComponent;
	
	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer GrantedTags;

	UPROPERTY(EditDefaultsOnly,Category="Tags")
	FGameplayTagContainer BlockedTags;
	
    UFUNCTION(BlueprintCallable,Category="Action")
	USActionComponent* GetOwningComponent() const
	{
		return ActionComponent;

    	// AActor* Actor = Cast<AActor>(GetOuter());
	    //
    	// return Actor->GetComponentByClass(USActionComponent::StaticClass());
	}
};
