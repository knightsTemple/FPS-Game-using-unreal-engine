// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "SWorldUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class TAKANASHI_HOSHINO_API USWorldUserWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	// UPROPERTY(meta=(BindWidget))
	// USizeBox* ParentSizeBox;

	UPROPERTY(BlueprintReadWrite,EditAnywhere,meta=(BindWidget))
	UProgressBar* HealthBar;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	UPROPERTY(EditAnywhere,Category="Widget")
	FVector WidgetOffset;
	
	UPROPERTY(BlueprintReadWrite, Category = "Widget",meta=(ExposeOnSpawn="true"))
	AActor* AttachedActor;
};
