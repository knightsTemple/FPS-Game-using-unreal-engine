#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

class USSaveGame;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASPlayerState*, PlayerState, int32, NewCredits, int32, Delta);

UCLASS()
class TAKANASHI_HOSHINO_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	// 使用ReplicatedUsing确保客户端收到更新时触发事件
	UPROPERTY(EditDefaultsOnly, Category = "Credits", ReplicatedUsing = "OnRep_Credits")
	int32 Credits;

	// 网络更新回调函数
	UFUNCTION()
	void OnRep_Credits(int32 OldCredits);

public:
	ASPlayerState();

	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits() const { return Credits; }

	// 客户端调用的添加 credits 函数，会通过 RPC 通知服务器
	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);

	// 客户端调用的减少 credits 函数，会通过 RPC 通知服务器
	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Delta);

	// 当 credits 变化时广播的事件
	UPROPERTY(BlueprintAssignable, Category = "Credits")
	FOnCreditsChanged OnCreditsChanged;

	// 保存玩家状态
	UFUNCTION(BlueprintNativeEvent)
	void SavePlayerState(USSaveGame* SaveObject);

	// 加载玩家状态
	UFUNCTION(BlueprintNativeEvent)
	void LoadPlayerState(USSaveGame* SaveObject);

protected:


	// 网络复制属性声明
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
