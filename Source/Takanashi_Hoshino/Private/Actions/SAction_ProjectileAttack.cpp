// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction_ProjectileAttack.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

USAction_ProjectileAttack::USAction_ProjectileAttack()
{
	HandSocketName = "hand_r_socket";
	AttackAnimDelay = 0.2f;
} 

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* Instigator)
{
	if (ensureAlways(ProjectileClass))
	{
		FVector HandLocation=Instigator->GetMesh()->GetSocketLocation("hand_r_socket");

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride=ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator=Instigator;

		FCollisionShape Shape;
		Shape.SetSphere(20.f);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(Instigator);

		FCollisionObjectQueryParams ObjParams;
		ObjParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjParams.AddObjectTypesToQuery(ECC_Pawn);
		ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);

		FVector TraceStart=Instigator->GetPawnViewLocation();
		FVector TraceEnd=TraceStart+Instigator->GetControlRotation().Vector()*5000.f;//GetControlRotation().Vector()获取的是控制器旋转体的前向向量，是玩家的朝向
		FHitResult HitRe;

		if (GetWorld()->SweepSingleByObjectType(HitRe, TraceStart, TraceEnd, FQuat::Identity, ObjParams, Shape, Params))
		{
			TraceEnd=HitRe.ImpactPoint;
		}

		FRotator ProjRotation=FRotationMatrix::MakeFromX(TraceEnd - HandLocation).Rotator();
		FTransform SpawnTM(ProjRotation, HandLocation);
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}
	StopAction(Instigator);
}

void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if (Character)
	{
		Character->PlayAnimMontage(AttackAnim);
		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		if (Character->HasAuthority())
		{
			FTimerHandle TimerHandle;
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUFunction(this, FName("AttackDelay_Elapsed"), Character);
			Character->GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, AttackAnimDelay, false); 
		}
		
	}
}


