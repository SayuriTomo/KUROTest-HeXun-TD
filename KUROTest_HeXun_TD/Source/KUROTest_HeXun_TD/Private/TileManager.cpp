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

	// 绑定所有棋盘
	for(AFaceManager* Face:FacesArray)
	{
		Face->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);
	}
}

// Called every frame
void ATileManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsGameStart)
	{
		ManageFaces();

		// 准备旋转
		if(bIsRotating)
		{
			if(CurrentRotation<45)
			{
				AddActorLocalRotation(FRotator(DegreeRequired.Pitch/45,DegreeRequired.Yaw/45,DegreeRequired.Roll/45));
				CurrentRotation += 1;
			}
			else
			{
				// 旋转完毕，轮到玩家或者ai回合
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

void ATileManager::ManageFaces()
{
	for(AFaceManager* Face:FacesArray)
	{
		if(Face->bIsActivating)
		{
			// 检测当前棋盘胜负或平局
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

			// 检测下一个棋盘是否符合
			CurrentFaceIndex = Face->NextFace;
			bool Flag = true;
			while (Flag)
			{
				if(!FacesArray[CurrentFaceIndex]->bIsFinished && Face != FacesArray[CurrentFaceIndex])
				{
					Flag = false;
				}
				else
				{
					UE_LOG(LogTemp,Warning,TEXT("棋盘已满"));
					CurrentFaceIndex = FMath::RandRange(0,5);
				}
			}
				
			UE_LOG(LogTemp,Warning,TEXT("Attempting to move face %i"),CurrentFaceIndex);
			DegreeRequired = Face->DegreeSet[CurrentFaceIndex];
			Face->bIsActivating = false;
			bIsRotating = true;
		}
	}
}

void ATileManager::MoveToAITurn()
{
	GetWorld()->GetTimerManager().ClearTimer(DelayTimerHandle);
	int BestScore = INT_MIN;
	int BestMove = -1;

	// 只检测当前棋盘的下一步胜负
	for (int i = 0; i < 9; ++i) {
		if (FacesArray[CurrentFaceIndex]->TilesArray[i]->BaseColor == FLinearColor::Gray)
		{
			int Score = 0;
			FacesArray[CurrentFaceIndex]->TilesArray[i]->BaseColor = FLinearColor::Red;
			int AIScore = abs(Evaluate(FacesArray[CurrentFaceIndex]->TilesArray));
			FacesArray[CurrentFaceIndex]->TilesArray[i]->BaseColor = FLinearColor::Gray;
			
			FacesArray[CurrentFaceIndex]->TilesArray[i]->BaseColor = FLinearColor::Blue;
			int RivalScore = abs(Evaluate(FacesArray[CurrentFaceIndex]->TilesArray));
			FacesArray[CurrentFaceIndex]->TilesArray[i]->BaseColor = FLinearColor::Gray;

			Score = AIScore<RivalScore?RivalScore:AIScore;

			if (Score > BestScore) {
				BestScore = Score;
				BestMove = i;
			}
		}
	}
	// 如果分不出胜负，则随机
	if(BestScore == 0)
	{
		UE_LOG(LogTemp,Warning,TEXT("下哪都行"));
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
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("必须下这"));
		FacesArray[CurrentFaceIndex]->TilesArray[BestMove]->ChangeColor(FLinearColor::Red,true);
		FacesArray[CurrentFaceIndex]->TilesArray[BestMove]->bIsSelected = true;
		FacesArray[CurrentFaceIndex]->TilesArray[BestMove]->BaseColor = FLinearColor::Red;
	}
	Cast<AKUROTest_HeXun_TDCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->bIsMyTurn = true;
}

void ATileManager::GenerateFaces()
{
	// 初始化棋盘
	for(AFaceManager* Face:FacesArray)
	{
		Face->bIsFinished = false;
		for(ATileActor* Tile:Face->TilesArray)
		{
			Tile->BaseColor=FLinearColor::Gray;
			Tile->ChangeColor(FLinearColor::Gray,true);
		}
	}
	Cast<AKUROTest_HeXun_TDCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->bIsGameStart = true;
	Cast<AKUROTest_HeXun_TDCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->bIsMyTurn = true;
	bIsGameStart = true;
	bIsRotating = false;
	CurrentRotation = 0.0f;
	PlayerScore = 0;
	EnemyScore = 0;
	DegreeRequired = FRotator(0,0,0);
	SetActorRotation(FRotator(0,0,0));
}

void ATileManager::EndGame()
{
	bIsGameStart = false;
	bIsEndGame = true;
	bIsRotating = false;
	CurrentRotation = 0.0f;
	SetActorRotation(FRotator(0,45,0));
	if(PlayerScore>=3)
	{
		LastWinner = 1;
	}
	else
	{
		LastWinner = 2;
	}
}

int ATileManager::Evaluate(TArray<ATileActor*> TilesArray)
{
	for (int i = 0; i < 3; ++i) {
		if (TilesArray[i * 3]->BaseColor == TilesArray[i * 3 + 1]->BaseColor &&
			TilesArray[i * 3 + 1]->BaseColor == TilesArray[i * 3 + 2]->BaseColor &&
			TilesArray[i * 3]->BaseColor != FLinearColor::Gray)
		{
			if(TilesArray[i * 3]->BaseColor == FLinearColor::Red)
			{
				return 1;
			}
			else if(TilesArray[i * 3]->BaseColor == FLinearColor::Blue)
			{
				return -1;
			}
		}
		
		if (TilesArray[i]->BaseColor == TilesArray[i + 3]->BaseColor &&
			TilesArray[i + 3]->BaseColor == TilesArray[i + 2 * 3]->BaseColor &&
			TilesArray[i]->BaseColor != FLinearColor::Gray)
		{
			if(TilesArray[i]->BaseColor == FLinearColor::Red)
			{
				return 1;
			}
			else if(TilesArray[i]->BaseColor == FLinearColor::Blue)
			{
				return -1;
			}
		}
	}
	
	if (TilesArray[0]->BaseColor == TilesArray[4]->BaseColor &&
		TilesArray[4]->BaseColor == TilesArray[8]->BaseColor &&
		TilesArray[0]->BaseColor != FLinearColor::Gray)
	{
		if(TilesArray[0]->BaseColor == FLinearColor::Red)
		{
			return 1;
		}
		else if(TilesArray[0]->BaseColor == FLinearColor::Blue)
		{
			return -1;
		}
	}
	
	if (TilesArray[2]->BaseColor == TilesArray[4]->BaseColor &&
		TilesArray[4]->BaseColor == TilesArray[6]->BaseColor &&
		TilesArray[2]->BaseColor != FLinearColor::Gray)
	{
		if(TilesArray[2]->BaseColor == FLinearColor::Red)
		{
			return 1;
		}
		else if(TilesArray[2]->BaseColor == FLinearColor::Blue)
		{
			return -1;
		}
	}
	
	return 0;
}
/*
void ATileManager::GenerateAIDecision()
{
	int BestScore = INT_MIN;
	int BestMove = -1;

	for (int i = 0; i < 9; ++i) {
		if (FacesArray[CurrentFaceIndex]->TilesArray[i]->BaseColor == FLinearColor::Gray)
		{
			FacesArray[CurrentFaceIndex]->TilesArray[i]->BaseColor = FLinearColor::Red;
			int Score = Minimax(FacesArray[CurrentFaceIndex]->TilesArray, true);
			FacesArray[CurrentFaceIndex]->TilesArray[i]->BaseColor = FLinearColor::Gray;

			if (Score > BestScore) {
				BestScore = Score;
				BestMove = i;
			}
		}
	}
	
	FacesArray[CurrentFaceIndex]->TilesArray[BestMove]->ChangeColor(FLinearColor::Red,true);
	FacesArray[CurrentFaceIndex]->TilesArray[BestMove]->bIsSelected = true;
	FacesArray[CurrentFaceIndex]->TilesArray[BestMove]->BaseColor = FLinearColor::Red;
	
	Cast<AKUROTest_HeXun_TDCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->bIsMyTurn = true;
}

int ATileManager::Minimax(TArray<ATileActor*> TilesArray, bool bIsMax)
{
	int Score = Evaluate(TilesArray);
	
	if (Score != 0||IsFull(TilesArray))
		return Score;

	if (bIsMax)
	{
		int BestScore = INT_MIN;
		for (int i = 0; i < 9; i++) {
			if (TilesArray[i]->BaseColor == FLinearColor::Gray)
			{
				TilesArray[i]->BaseColor = FLinearColor::Red;
				BestScore = FMath::Max(BestScore, Minimax(TilesArray, false));
				TilesArray[i]->BaseColor = FLinearColor::Gray;
			}
		}
		return BestScore;
	}
	else
	{
		int BestScore = INT_MAX;
		for (int i = 0; i < 9; i++) {
			if (TilesArray[i]->BaseColor == FLinearColor::Gray)
			{
				TilesArray[i]->BaseColor = FLinearColor::Blue;
				BestScore = FMath::Min(BestScore, Minimax(TilesArray, true));
				TilesArray[i]->BaseColor = FLinearColor::Gray;
			}
		}
		return BestScore;
	}
}

bool ATileManager::IsFull(TArray<ATileActor*> TilesArray)
{
	for(int i=0; i<TilesArray.Num(); i++)
	{
		if(TilesArray[i]->BaseColor == FLinearColor::Gray)
		{
			return false;
		}
	}
	return true;
}*/