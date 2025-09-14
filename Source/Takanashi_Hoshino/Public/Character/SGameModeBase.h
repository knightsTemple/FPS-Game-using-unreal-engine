// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class USSaveGame;
class UEnvQueryInstanceBlueprintWrapper;

class UEnvQuery;
/**
 * 
 */
UCLASS()
class TAKANASHI_HOSHINO_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ASGameModeBase();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
	virtual void StartPlay() override;

	//当有新玩家加入游戏（包括本地玩家启动游戏、远程玩家连接到服务器）时，引擎会自动调用这个函数
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)override;

	UFUNCTION(Exec)
	void KillAll();

	virtual void OnActorKilled(AActor* VictimActor, AActor* Killer);

	UFUNCTION(BlueprintCallable,Category="SameGame")
	void WriteSaveGame();

	void LoadSaveGame();

protected:

	FString SlotName;
	
	UPROPERTY()
	USSaveGame* CurrentSaveGame;
	
	UPROPERTY(EditAnywhere,Category="AI")
	TSubclassOf<AActor> MinionClass;
	
	UPROPERTY(EditAnywhere,Category="AI")
	UEnvQuery* SpawnBotQuery;
	
	UPROPERTY(EditDefaultsOnly,Category="AI")
	float SpawnTimerInterval ;

	UPROPERTY(EditDefaultsOnly,Category="AI")
	UCurveFloat* DifficultyCurve;
	
	FTimerHandle TimerHandle_SpawnBots;

	UPROPERTY(EditDefaultsOnly,Category="AI")
	int CreditPerKill;

	UPROPERTY(EditDefaultsOnly,Category="AI")
	int DesiredPowerUpCount;

	UPROPERTY(EditDefaultsOnly,Category="AI")
	int RequiredPowerUpDistance;

	UFUNCTION()
	void SpawnBotTimerElapsed();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION()
	void RespawnPlayerElasped(AController* Controller);
	
};
