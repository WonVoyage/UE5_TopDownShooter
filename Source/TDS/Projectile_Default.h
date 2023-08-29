#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISense_Damage.h"
#include "Net/UnrealNetwork.h"

#include "My_Types.h"
#include "Projectile_Default.generated.h"

//-------------------------------------------------------------------------------------------------------------
UCLASS()
class TDS_API AProjectile_Default : public AActor
{
	GENERATED_BODY()
	
public:
	// Functions
	AProjectile_Default();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable) void Init(FProjectile_Info init_param);
	virtual void Impact_Projectile();

	UFUNCTION(BlueprintCallable) virtual void Collision_Hit(class UPrimitiveComponent *hit_component, AActor *other_actor, UPrimitiveComponent *other_component, FVector normal_impulse, const FHitResult &hit);
	UFUNCTION(BlueprintCallable) void Collision_Begin_Overlap(UPrimitiveComponent *overlapped_component, AActor *other_actor, UPrimitiveComponent *other_component, int32 other_body_index, bool from_sweep, const FHitResult &sweep_result);
	UFUNCTION(BlueprintCallable) void Collision_End_Overlap(UPrimitiveComponent *overlapped_component, AActor *other_actor, UPrimitiveComponent *other_component, int32 other_body_index);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable) void Init_Visible_Projectile_Multicast(UStaticMesh *mesh);

	// Variables
	FProjectile_Info Projectile_Settings;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") UStaticMeshComponent *Bullet_Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") USphereComponent *Bullet_Collision_Sphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") UProjectileMovementComponent *Bullet_Projectile_Movement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components") UParticleSystemComponent *Bullet_FX;
	
protected:
	// Functions
	virtual void BeginPlay() override;
};
//-------------------------------------------------------------------------------------------------------------
 