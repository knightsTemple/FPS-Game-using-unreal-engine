// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Hoshino.h"
#include"GameFramework/PlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include"DrawDebugHelpers.h"
#include "Actions/SActionComponent.h"
#include "Character/CharacterStats.h"
#include "Component/SAttributeComponent.h"
#include "Component/SInteractionComponent.h"
#include "Evaluation/Blending/MovieSceneBlendType.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Perception/AISense_Sight.h"


class UEnhancedInputLocalPlayerSubsystem;
// Sets default values
AHoshino::AHoshino()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//创建一个相机组件
	CameraBoom =CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength=500.f;
	CameraBoom->SetRelativeLocation(FVector(0,0,50));
	
	PlayerCamera=CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(CameraBoom);
	bUseControllerRotationPitch=false;
	bUseControllerRotationYaw=false;
	bUseControllerRotationRoll=false;
	CameraBoom->bUsePawnControlRotation=true;
	PlayerCamera->bUsePawnControlRotation=false;
	
	
	GetCharacterMovement()->bOrientRotationToMovement=true;
	GetCharacterMovement()->RotationRate=FRotator(0,500,0);

	HoldingComponent=CreateDefaultSubobject<USceneComponent>(TEXT("HoldingComponent"));
	HoldingComponent->SetRelativeLocation(FVector(50.f,0,0));
	HoldingComponent->SetupAttachment(RootComponent);
	bCanMove=true;
	bIsInspecting=false;
	bHoldingItem=false;

	setup_stimulus();

	InteractionComponent=CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComponent"));

	AttributeComp=CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));

	ActionComp=CreateDefaultSubobject<USActionComponent>(TEXT("ActionComp"));
	
	HandSocketName=TEXT("hand_r_socket");
}

void AHoshino::ToggleMovement()
{
	bCanMove=!bCanMove;
	bIsInspecting=!bIsInspecting;
	CameraBoom->bUsePawnControlRotation=!CameraBoom->bUsePawnControlRotation;
	if (CurrentItem)
	{
		CurrentItem->SetActorRotation(GetControlRotation());
	}
	
}

void AHoshino::ToggleItemPickUp()
{
	if (CurrentItem)
	{
		bHoldingItem=!bHoldingItem;
		CurrentItem->PickUp();
		if (!bHoldingItem)
		{
			CurrentItem=NULL;
		}
	}
}

void AHoshino::TakeDamage_Implementation(float Damage)
{
	IHPInterface::TakeDamage_Implementation(Damage);
	GEngine->AddOnScreenDebugMessage(-1,10,FColor::Red,FString::Printf(TEXT("TakeDamage")));
}

// Called when the game starts or when spawned
void AHoshino::BeginPlay()
{
	Super::BeginPlay();
	APlayerCameraManager *const CameraManager=GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	if (CameraManager)
	{
		CameraManager->ViewPitchMax=50.f;
		CameraManager->ViewPitchMin=-50.f;
	
		PitchMax=CameraManager->ViewPitchMax;
		PitchMin=CameraManager->ViewPitchMin;
	}

	AttributeComp->OnHealthChanged.AddDynamic(this,&AHoshino::OnHealthChanged);
	
}

void AHoshino::Move(const FInputActionValue& Value)
{
	FVector2D MoveValue=Value.Get<FVector2D>();
	if (Controller&&bCanMove)
	{
		const FRotator Rotation=Controller->GetControlRotation();
		const FRotator YawRotation(0,Rotation.Yaw,0);
		//const FVector ForwardDirection=FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector ForwardDirection=YawRotation.Quaternion().GetForwardVector();
		const FVector RightDirection=FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(ForwardDirection,MoveValue.Y);
		AddMovementInput(RightDirection,MoveValue.X);
		
	}
}

void AHoshino::Look(const FInputActionValue& Value)
{
	FVector2D LookValue=Value.Get<FVector2D>();
	AddControllerYawInput(-LookValue.X);
	AddControllerPitchInput(LookValue.Y);
}

void AHoshino::Jumping()
{
	if (bCanMove)
	{
		GetCharacterMovement()->JumpZVelocity=JumpVelocity;
		Jump();
	}
}

void AHoshino::onAction()
{
	if (CurrentItem&&!bIsInspecting)
	{
		ToggleItemPickUp();
	}
}

void AHoshino::OnInspect()
{
	if (bHoldingItem)
	{
		LastRotator=GetControlRotation();
		ToggleMovement();
	}
	else
	{
		bIsInspecting=true;
	}
}

void AHoshino::OnInspectReleased()
{
	if (bIsInspecting&&bHoldingItem)
	{
		GetController()->SetControlRotation(LastRotator);
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax=PitchMax;
		GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin=PitchMin;
		ToggleMovement();
	}
	else
	{
		bIsInspecting=false;
	}
}

void AHoshino::Interact()
{
	if (InteractionComponent)
	{
		InteractionComponent->PrimaryInteract();
	}
}

void AHoshino::SetBiggerView()
{
	start=PlayerCamera->GetComponentLocation();
	ForwardVector=PlayerCamera->GetForwardVector();
	End=ForwardVector*1000.f+start;
	DefaultComponentQueryParams.AddIgnoredActor(this);

	//DrawDebugLine(GetWorld(),start,End,FColor::Red,false,1.f,0,1.f);

	if (!bHoldingItem)
	{
		if (GetWorld()->LineTraceSingleByChannel(Hit,start,End,ECC_Visibility,DefaultComponentQueryParams,DefaultResponseParams))
		{
			if (Hit.GetActor()&&Hit.GetActor()->GetClass()->IsChildOf(APickUp::StaticClass())) 
			{
				CurrentItem=Cast<APickUp>(Hit.GetActor());

				//接口实现伤害
				
				// if ( Hit.GetActor()->Implements<IHPInterface>())
				// {
				// IHPInterface *HPInterface=Cast<IHPInterface>(Hit.GetActor());
				// 	if (HPInterface)
				// 	{
				// 		HPInterface->TakeDamage(10.f);
				// 	}
				// }
			}
		}
		else
		{
			CurrentItem=NULL;
		}
	}

	if (bIsInspecting)
	{
		if (bHoldingItem)
		{
			PlayerCamera->SetFieldOfView(FMath::Lerp(PlayerCamera->FieldOfView,90.f,0.1f));
			HoldingComponent->SetRelativeLocation(FVector(0,50.f,50.f));
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMax=179.90000002f;
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->ViewPitchMin=-179.90000002f;
			CurrentItem->RotateActor();
		}
		else
		{
			
			PlayerCamera->SetFieldOfView(FMath::Lerp(PlayerCamera->FieldOfView,45.f,0.1f));
			
		}
	}
	else
	{
		PlayerCamera->SetFieldOfView(FMath::Lerp(PlayerCamera->FieldOfView,90.f,0.1f));
		if (bHoldingItem)
		{
			HoldingComponent->SetRelativeLocation(FVector(50.f,0,0));
		}
	}
}

FVector AHoshino::GetPawnViewLocation() const
{
	return PlayerCamera->GetComponentLocation();
}

// Called every frame
void AHoshino::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//右键按下时，摄像机视角变大
	SetBiggerView();

	// //长按，如果按下时间超过0.5秒，则在摄像机前方生成一个绿色的球体，提示将要移动的位置
	// HoldOn(DeltaTime);
}


void AHoshino::setup_stimulus()
{
	Stimulus=CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterWithPerceptionSystem();
}

void AHoshino::Exit_game()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}


void AHoshino::primaryAttack()
{
	ActionComp->StartActionByName(this, "PrimaryAttack");
}


void AHoshino::SprintStart()
{
	ActionComp->StartActionByName(this, "Sprint");
}

void AHoshino::SprintStop()
{
	ActionComp->StopActionByName(this, "Sprint");
}

void AHoshino::BlackHoleAttack()
{
	ActionComp->StartActionByName(this, "Dash");
}


// Called to bind functionality to input
void AHoshino::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	if (UEnhancedInputComponent * EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHoshino::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AHoshino::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AHoshino::Jumping);
        EnhancedInputComponent->BindAction(Action,ETriggerEvent::Triggered,this,&AHoshino::onAction);
		EnhancedInputComponent->BindAction(Inspect,ETriggerEvent::Started,this,&AHoshino::OnInspect);
		EnhancedInputComponent->BindAction(Inspect,ETriggerEvent::Completed,this,&AHoshino::OnInspectReleased);
		EnhancedInputComponent->BindAction(PrimaryAttack,ETriggerEvent::Started,this,&AHoshino::primaryAttack);
		EnhancedInputComponent->BindAction(PrimaryInteraction,ETriggerEvent::Started,this,&AHoshino::Interact);
		EnhancedInputComponent->BindAction(BlackHoleAttackAction,ETriggerEvent::Completed,this,&AHoshino::BlackHoleAttack);
		EnhancedInputComponent->BindAction(Sprint,ETriggerEvent::Started,this,&AHoshino::SprintStart);
		EnhancedInputComponent->BindAction(Sprint,ETriggerEvent::Completed,this,&AHoshino::SprintStop);
	}

	// PlayerInputComponent->BindAction("Action",IE_Pressed,this,&AHoshino::onAction);
	// PlayerInputComponent->BindAction("Inspect",IE_Pressed,this,&AHoshino::OnInspect);
	// PlayerInputComponent->BindAction("Inspect",IE_Released,this,&AHoshino::OnInspectReleased);
	PlayerInputComponent->BindAction("ExitGame",IE_Pressed,this,&AHoshino::Exit_game);

}

void AHoshino::HealSelf(float Amount)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}

void AHoshino::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
   float Delta)
{
	
	if (NewHealth <= 0.f&&Delta<0.f)
	{
		APlayerController* PC=Cast<APlayerController>(GetController());
		DisableInput(PC);

		SetLifeSpan(5.0f); //设置角色在死亡后5秒后自动销毁
	}
}

