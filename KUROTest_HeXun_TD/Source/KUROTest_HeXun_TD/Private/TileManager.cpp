// Fill out your copyright notice in the Description page of Project Settings.


#include "KUROTest_HeXun_TD/Public/TileManager.h"

#include "Components/TextBlock.h"
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
		Face->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);\
		for(ATileActor* TileActor:Face->TilesArray)
		{
			//TileActor->SetHidden(true);
		}
	}
}

// Called every frame
void ATileManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsGameStart)
	{
		for(AFaceManager* Face:FacesArray)
		{
			if(Face->bIsActivating)
			{
				FLinearColor FaceWinnerColor = Face->CheckFaceWin();
				if(FaceWinnerColor==FLinearColor::Blue||FaceWinnerColor==FLinearColor::Red)
				{
					Face->bIsFinished = true;
					for(ATileActor* TileActor:Face->TilesArray)
					{
						TileActor->ChangeColor(FaceWinnerColor,true);
						TileActor->BaseColor = FaceWinnerColor;
					}
					if(FaceWinnerColor==FLinearColor::Blue)PlayerScore+=1;
					else if(FaceWinnerColor==FLinearColor::Red)EnemyScore+=1;

					if(PlayerScore>=3||EnemyScore>=3)
					{
						EndGame();
					}
				}
				if(Face->CheckFaceDraw())
				{
					Face->bIsFinished = true;
				}
				CurrentFaceIndex = Face->NextFace;
				
				bool Flag = true;
				while (Flag)
				{
					if(!FacesArray[CurrentFaceIndex]->bIsFinished)
					{
						Flag = false;
					}
					else
					{
						CurrentFaceIndex = FMath::RandRange(0,5);
					}
				}
				
				UE_LOG(LogTemp,Warning,TEXT("Attempting to move face %i"),CurrentFaceIndex);
				DegreeRequired = Face->DegreeSet[CurrentFaceIndex];
				UE_LOG(LogTemp,Warning,TEXT("0p%f,y%f,r%f"),Face->DegreeSet[0].Pitch,Face->DegreeSet[0].Yaw,Face->DegreeSet[0].Roll)
				UE_LOG(LogTemp,Warning,TEXT("1p%f,y%f,r%f"),Face->DegreeSet[1].Pitch,Face->DegreeSet[1].Yaw,Face->DegreeSet[1].Roll)
				UE_LOG(LogTemp,Warning,TEXT("2p%f,y%f,r%f"),Face->DegreeSet[2].Pitch,Face->DegreeSet[2].Yaw,Face->DegreeSet[2].Roll)
				UE_LOG(LogTemp,Warning,TEXT("3p%f,y%f,r%f"),Face->DegreeSet[3].Pitch,Face->DegreeSet[3].Yaw,Face->DegreeSet[3].Roll)
				UE_LOG(LogTemp,Warning,TEXT("4p%f,y%f,r%f"),Face->DegreeSet[4].Pitch,Face->DegreeSet[4].Yaw,Face->DegreeSet[4].Roll)
				UE_LOG(LogTemp,Warning,TEXT("5p%f,y%f,r%f"),Face->DegreeSet[5].Pitch,Face->DegreeSet[5].Yaw,Face->DegreeSet[5].Roll)
				
				UE_LOG(LogTemp,Warning,TEXT("p%f,y%f,r%f"),DegreeRequired.Pitch,DegreeRequired.Yaw,DegreeRequired.Roll);
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

void ATileManager::GenerateFaces()
{
	for(AFaceManager* Face:FacesArray)
	{
		for(ATileActor* TileActor:Face->TilesArray)
		{
			TileActor->BaseColor=FLinearColor::Gray;
			TileActor->ChangeColor(FLinearColor::Gray,true);
			//TileActor->SetHidden(false);
		}
	}
	Cast<AKUROTest_HeXun_TDCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->bIsGameStart = true;
	bIsGameStart = true;
	bIsRotating = false;
	CurrentRotation = 0.0f;
	PlayerScore = 0;
	EnemyScore=0;
	DegreeRequired = FRotator(0);
	SetActorRotation(FRotator(0));
}

void ATileManager::EndGame()
{
	bIsGameStart = false;
	bIsEndGame = true;
	bIsRotating=false;
	CurrentRotation = 0.0f;
	SetActorRotation(FRotator(0,0,0));
	for(AFaceManager* Face:FacesArray)
	{
		///for(ATileActor* TileActor:Face->TilesArray)
		//{
			//TileActor->SetHidden(true);
		//}
	}
	
	LastWinner = PlayerScore>=3?"玩家":"敌人";
}
