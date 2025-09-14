// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/GameplayStatics.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!IsValid(AttachedActor))
	{
		RemoveFromParent();
		return; //如果附加的Actor无效，则不执行后续操作
	}
	FVector2D ScreenPosition;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation()+WidgetOffset, ScreenPosition))
	{
		float scale=UWidgetLayoutLibrary::GetViewportScale(GetWorld());

		ScreenPosition/= scale;

		if (HealthBar)
		{
			HealthBar->SetRenderTranslation(ScreenPosition);//设置父容器的渲染位置
		}
	}
}
