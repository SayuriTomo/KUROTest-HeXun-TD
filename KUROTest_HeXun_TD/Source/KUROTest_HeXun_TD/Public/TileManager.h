// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FaceManager.h"
#include "Components/Button.h"
#include "GameFramework/Actor.h"
#include "TileManager.generated.h"

UCLASS()
class KUROTEST_HEXUN_TD_API ATileManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileManager();
	UPROPERTY(EditAnywhere)UStaticMeshComponent* TileManagerMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void MoveToAITurn();
	bool bIsAITurn = false;


	FTimerHandle DelayTimerHandle;
	
	float DelayTime = 5;
	int CurrentFaceIndex;
	bool bIsRotating = false;
	FRotator DegreeRequired;
	float CurrentRotation = 0.0f;
	UPROPERTY(EditAnywhere)TArray<AFaceManager*> FacesArray;
};
