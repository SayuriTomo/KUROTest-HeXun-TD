// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileActor.h"
#include "GameFramework/Actor.h"
#include "FaceManager.generated.h"

UENUM()
enum ENextFace
{
	FACE_UP,
	FACE_DOWN,
	FACE_LEFT,
	FACE_RIGHT,
	FACE_OPPOSITE
};

UCLASS()
class KUROTEST_HEXUN_TD_API AFaceManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFaceManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GenerateTiles();
	FLinearColor CheckFaceWin();
	bool CheckFaceDraw();
	bool bIsActivating = false;
	bool bIsFinished = false;
	int NextFace;
	UPROPERTY(EditAnywhere)TArray<FRotator> DegreeSet;
	UPROPERTY(EditAnywhere)TArray<ATileActor*> TilesArray;
};
