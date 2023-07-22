#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "World_Item_Default.generated.h"

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TDS_API AWorld_Item_Default : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorld_Item_Default();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
//-------------------------------------------------------------------------------------------------------------
