// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/SInteractionComponent.h"

#include "Interface/SGameplayInterface.h"

static TAutoConsoleVariable<bool> CVarDebugDraw(TEXT("su.DebugDraw"), false, TEXT("Enable debug drawing for interaction."), ECVF_Cheat);
// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	TraceRadius=30.0f;
	TraceDistance=500.f;

	CollisionChannel=ECC_WorldDynamic;
}

void USInteractionComponent::PrimaryInteract()
{
	ServiceInteract(FocusedActor);
}
	
void USInteractionComponent::ServiceInteract_Implementation(AActor* InFocus)
{
	if (InFocus == nullptr)
	{
		return;
	}
	APawn* Pawn=Cast<APawn>(GetOwner());
	ISGameplayInterface::Execute_Interact(InFocus, Pawn);
}

// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void USInteractionComponent::FindBestInteractable()
{
	bool bDebugDraw=CVarDebugDraw.GetValueOnGameThread();
	FCollisionObjectQueryParams TraceParams;
	TraceParams.AddObjectTypesToQuery(CollisionChannel);

	AActor* Owner = GetOwner();
	
	FVector EyeLocation;
	FRotator EyeRotation;
	
	Owner->GetActorEyesViewPoint(EyeLocation, EyeRotation);//获取玩家眼睛的位置储存在EyeLocation中，获取玩家眼睛的朝向储存在EyeRotation
	// FHitResult Hit;
	// bool bBlocking =GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, TraceParams);
	FVector End=EyeLocation+(EyeRotation.Vector()*TraceDistance);
	TArray<FHitResult> Hits;
	bool bBlocking=GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, TraceParams, FCollisionShape::MakeSphere(TraceRadius));
	FColor DebugColor = bBlocking ? FColor::Green : FColor::Red;

	FocusedActor=nullptr;
	
	for (FHitResult Hit : Hits)
	{
		if (bDebugDraw)
		{
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 30.f, 32, DebugColor, false, 2.f);
		}
		
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			if (HitActor->Implements<USGameplayInterface>())
			{
				FocusedActor = HitActor;
				break;
			}
		}
	}
	if (FocusedActor)
	{
		if (DefaultWidgetInstance==nullptr&&ensure(DefaultWidgetClass))
		{
			DefaultWidgetInstance = CreateWidget<USUserDefaultWidget>(GetWorld(), DefaultWidgetClass);
		}
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->AttachedActor=FocusedActor;

			if (!DefaultWidgetInstance->IsInViewport())
			{
				DefaultWidgetInstance->AddToViewport();
			}
			
		}
	}
	else
	{
		if (DefaultWidgetInstance)
		{
			DefaultWidgetInstance->RemoveFromParent();
		}
	}
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* MyPawn=Cast<APawn>(GetOwner());
	if (MyPawn->IsLocallyControlled())
	{
		FindBestInteractable();
	}
	
}

