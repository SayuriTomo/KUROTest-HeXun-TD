// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileActor.generated.h"

UCLASS()
class KUROTEST_HEXUN_TD_API ATileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileActor();

	UPROPERTY(EditAnywhere)UStaticMeshComponent* TileMesh;

	UPROPERTY(EditAnywhere)UMaterialInterface* MaterialClass;

	UPROPERTY()UMaterialInstanceDynamic* MaterialInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ChangeColor(FLinearColor ColorToChange, bool bIsBaseOrEdge);

};
