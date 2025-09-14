// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SUserDefaultWidget.generated.h"

class USizeBox;
/**
 * 
 */
UCLASS()
class TAKANASHI_HOSHINO_API USUserDefaultWidget : public UUserWidget
{
	GENERATED_BODY()
protected:

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(BindWidget))
	USizeBox* ParentSizeBox;
	
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	UPROPERTY(EditAnywhere,Category="Widget")
	FVector WidgetOffset;
	
	UPROPERTY(BlueprintReadOnly, Category = "Widget")
	AActor* AttachedActor;
};
