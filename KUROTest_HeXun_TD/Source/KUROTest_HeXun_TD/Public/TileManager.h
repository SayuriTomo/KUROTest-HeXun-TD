// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FaceManager.h"
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

	UFUNCTION(BlueprintCallable)void GenerateFaces();
	int PlayerScore;
	int EnemyScore;
	FTimerHandle DelayTimerHandle;
	bool bIsGameStart = false;
	float DelayTime = 2;
	int CurrentFaceIndex;
	int LastWinner = 0;

	void EndGame();
	bool bIsEndGame = false;
	bool bIsRotating = false;
	FRotator DegreeRequired;
	float CurrentRotation = 0.0f;
	UPROPERTY(EditAnywhere)TArray<AFaceManager*> FacesArray;

	
	void GenerateAIDecision();
	
	int Evaluate(TArray<ATileActor*> Board);

	int Minimax(TArray<ATileActor*> Board, bool bIsMax);

	bool IsFull(TArray<ATileActor*> Board);

	//UFUNCTION(BlueprintCallable)void LeftRotate();
	//UFUNCTION(BlueprintCallable)void DownRotate();
};
