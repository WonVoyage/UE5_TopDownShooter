#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TDSGameMode.generated.h"

//-------------------------------------------------------------------------------------------------------------
UCLASS(minimalapi)
class ATDSGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATDSGameMode();

	UFUNCTION(BlueprintNativeEvent) void BP_Dead();
};
//-------------------------------------------------------------------------------------------------------------
