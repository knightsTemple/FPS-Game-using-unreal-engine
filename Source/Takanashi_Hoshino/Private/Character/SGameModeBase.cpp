// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SGameModeBase.h"

#include "EngineUtils.h"
#include "AINew/SAICharacter.h"
#include "Character/Hoshino.h"
#include "Character/SPlayerState.h"
#include "Character/SSaveGame.h"
#include "Component/SAttributeComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Interface/SGameplayInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

static TAutoConsoleVariable<bool>CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots."),ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval=2.0f;

	CreditPerKill=20;
	DesiredPowerUpCount=10;
	RequiredPowerUpDistance=2000;

	PlayerStateClass= ASPlayerState::StaticClass();

	SlotName="SaveGame01";
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	ASPlayerState* PS=NewPlayer->GetPlayerState<ASPlayerState>();
	if (PS)
	{
		PS->LoadPlayerState(CurrentSaveGame);
	}
}

void ASGameModeBase::KillAll()
{
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		USAttributeComponent* AttributeComp =USAttributeComponent::GetAttributes(Bot);
		if (AttributeComp&& AttributeComp->IsAlive())
		{    
			AttributeComp->Kill(this); // Kill the bot
		}
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	if (!CVarSpawnBots.GetValueOnGameThread())
	{
		return;
	}
	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It)
	{
		ASAICharacter* Bot = *It;
		USAttributeComponent* AttributeComp =USAttributeComponent::GetAttributes(Bot);
		if (AttributeComp&& AttributeComp->IsAlive())
		{
			NrOfAliveBots++;
		}
	}

	float MaxBots = 10.0f; // Maximum number of bots allowed
	if (DifficultyCurve)
	{
		MaxBots=DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	}
	
	if (NrOfAliveBots >= MaxBots)
	{
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance= UEnvQueryManager::RunEQSQuery(this,SpawnBotQuery,this,EEnvQueryRunMode::RandomBest5Pct,nullptr);

	if (QueryInstance)
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this,&ASGameModeBase::OnQueryCompleted);
	}
}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus!=EEnvQueryStatus::Success)
	{
		UE_LOG(LogTemp,Warning,TEXT("EQS Query Failed!"));
		return;
	}
    
	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(MinionClass,Locations[0], FRotator::ZeroRotator);

		DrawDebugSphere(GetWorld(), Locations[0],  50.0f, 5.0f, FColor::Green, true);
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer)
{
	AHoshino* Player=Cast<AHoshino>(VictimActor);
	if (Player)
	{
		FTimerHandle TimerHandle_RespawnPlayer;
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUFunction(this, FName("RespawnPlayerElasped"), Player->GetController());
		float RespawnTime = 2.0f; // Set the respawn time
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnPlayer, TimerDelegate, RespawnTime, false);
	}

	APawn* KillPawn= Cast<APawn>(Killer);
	if (KillPawn)
	{
		ASPlayerState* PS=KillPawn->GetPlayerState<ASPlayerState>();
		if (PS)
		{
			PS->AddCredits(CreditPerKill);
		}
	}
	
}

void ASGameModeBase::WriteSaveGame()
{

	for (int32 i=0;i<GameState->PlayerArray.Num();i++)
	{
		ASPlayerState* PS=Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if (PS)
		{
			PS->SavePlayerState(CurrentSaveGame);
			break; //只有一个玩家状态需要保存
		}
	}

	CurrentSaveGame->SavedActors.Empty();

	for (FActorIterator It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if (!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}
		FActorSaveData SaveData;
		SaveData.ActorName = Actor->GetName();
		SaveData.Transform = Actor->GetTransform();

		//创建一个MemWriter对象，后续所有需要保存的数据都会通过它写入到SaveData.ByteData数组中，相当于打开一个 “内存文件” 准备写入数据。
        FMemoryWriter MemWriter(SaveData.ByteData);

		//创建一个 “归档器”Ar，它是数据序列化的 “中间代理人”，负责将对象数据转换为二进制格式，并通过MemWriter写入ByteData
		FObjectAndNameAsStringProxyArchive Ar(MemWriter,true);

		//表示这是保存操作（true为保存，false为加载）
		Ar.ArIsSaveGame = true;

		/*触发 Actor 的序列化流程。此时 Actor 会遍历自身所有标记了UPROPERTY(SaveGame)的属性（如宝箱的bLidOpened、位置Location等），
		并通过Ar归档器将这些属性的当前值转换为二进制数据，最终存入SaveData.ByteData中。*/
		Actor->Serialize(Ar);

		CurrentSaveGame->SavedActors.Add(SaveData);
	}
	UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SlotName, 0);
	
}

void ASGameModeBase::LoadSaveGame()
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName,0))
	{
		CurrentSaveGame=Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName,0));
		if (CurrentSaveGame==nullptr)
		{
			UE_LOG(LogTemp,Warning,TEXT("Failed to load save game from slot %s"),*SlotName);
			return;
		}
		UE_LOG(LogTemp,Log,TEXT("Loaded save game from slot %s"),*SlotName);

		for (FActorIterator It(GetWorld()); It; ++It)
		{
			AActor* Actor = *It;
			if (!Actor->Implements<USGameplayInterface>())
			{
				continue;
			}
			for (FActorSaveData ActorData: CurrentSaveGame->SavedActors)
			{
				if (ActorData.ActorName==Actor->GetName())
				{
					Actor->SetActorTransform(ActorData.Transform);

					FMemoryReader MemReader(ActorData.ByteData);
					FObjectAndNameAsStringProxyArchive Ar(MemReader,true);
					
					Ar.ArIsSaveGame = true;
					
					Actor->Serialize(Ar);

					/*Serialize 函数的主要作用是恢复 Actor 中标记了 UPROPERTY(SaveGame) 的属性值
					（比如宝箱的 bLidOpened 状态、位置等），但它不会自动触发这些属性变化带来的 “副作用”。所以要手动调用*/
					ISGameplayInterface::Execute_OnActorLoaded(Actor);
					break;
				}
			}
		}
	}
	else
	{
		CurrentSaveGame=Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));

		UE_LOG(LogTemp,Log,TEXT("Created new save game in slot %s"),*SlotName);
	}
	
}

void ASGameModeBase::RespawnPlayerElasped(AController* Controller)
{
	if (ensure(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}
