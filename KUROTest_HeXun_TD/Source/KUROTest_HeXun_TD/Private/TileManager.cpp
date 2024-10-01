// Fill out your copyright notice in the Description page of Project Settings.


#include "KUROTest_HeXun_TD/Public/TileManager.h"

#include "KUROTest_HeXun_TD/KUROTest_HeXun_TDCharacter.h"

// Sets default values
ATileManager::ATileManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileManagerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile Manager Mesh"));
	SetRootComponent(TileManagerMesh);
	
}

// Called when the game starts or when spawned
void ATileManager::BeginPlay()
{
	Super::BeginPlay();
	
	for(AFaceManager* Face:FacesArray)
	{
		Face->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);
	}
	
}

// Called every frame
void ATileManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for(AFaceManager* Face:FacesArray)
	{
		if(Face->bIsActivating)
		{
			FLinearColor FaceWinnerColor = Face->bIsWin();
			if(FaceWinnerColor==FLinearColor::Blue||FaceWinnerColor==FLinearColor::Red)
			{
				Face->bIsFinished = true;
				for(ATileActor* TileActor:Face->TilesArray)
				{
					TileActor->ChangeColor(FaceWinnerColor,true);
				}
			}
			CurrentFaceIndex = Face->NextFace;
			
			//UE_LOG(LogTemp,Warning,TEXT("Attempting to move face %s"),*FacesArray[CurrentFaceIndex]->GetFName().ToString());
			DegreeRequired = Face->DegreeSet[CurrentFaceIndex];

			//UE_LOG(LogTemp,Warning,TEXT("p%f,y%f,r%f"),DegreeRequired.Pitch,DegreeRequired.Yaw,DegreeRequired.Roll);
			Face->bIsActivating = false;
			bIsRotating = true;
		}
	}

	if(bIsRotating)
	{
		if(CurrentRotation<45)
		{
			AddActorLocalRotation(FRotator(DegreeRequired.Pitch/45,DegreeRequired.Yaw/45,DegreeRequired.Roll/45));
			CurrentRotation += 1;
		}
		else
		{
			
			bIsRotating = false;;
			CurrentRotation = 0;
			
			if(!Cast<AKUROTest_HeXun_TDCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->bIsMyTurn)
			{
				GetWorld()->GetTimerManager().SetTimer(DelayTimerHandle, this, &ATileManager::MoveToAITurn, DelayTime, true);
			}
		}
	}

	
}

void ATileManager::MoveToAITurn()
{
	GetWorld()->GetTimerManager().ClearTimer(DelayTimerHandle);
	bool Flag = true;
	int RandTile = 0;
	while (Flag)
	{
		RandTile = FMath::RandRange(0,8);
		if(FacesArray[CurrentFaceIndex]->TilesArray[RandTile]->BaseColor == FLinearColor::Gray)
		{
			Flag = false;
		}
	}
	FacesArray[CurrentFaceIndex]->TilesArray[RandTile]->ChangeColor(FLinearColor::Red,true);
	FacesArray[CurrentFaceIndex]->TilesArray[RandTile]->bIsSelected = true;
	FacesArray[CurrentFaceIndex]->TilesArray[RandTile]->BaseColor = FLinearColor::Red;

	Cast<AKUROTest_HeXun_TDCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->bIsMyTurn = true;
}
