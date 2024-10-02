// Fill out your copyright notice in the Description page of Project Settings.


#include "FaceManager.h"

#include "IContentBrowserSingleton.h"

// Sets default values
AFaceManager::AFaceManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFaceManager::BeginPlay()
{
	Super::BeginPlay();

	//绑定所有棋片
	for(ATileActor* Tile:TilesArray)
	{
		Tile->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);
	}
}

// Called every frame
void AFaceManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ManageTiles();
}

void AFaceManager::ManageTiles()
{
	for(int i = 0;i<TilesArray.Num();i++)
	{
		if(TilesArray[i]->bIsSelected)
		{
			UE_LOG(LogTemp,Warning,TEXT("face tile %i"),i)
			TilesArray[i]->bIsSelected = false;
			if(TilesArray[i]->NextFace.Num()>1)
			{
				NextFace = TilesArray[i]->NextFace[FMath::RandRange(0,1)];
			}
			else
			{
				NextFace = TilesArray[i]->NextFace[0];
			}
			bIsActivating = true;
			UE_LOG(LogTemp,Warning,TEXT("FACE Attempting to move face %i"),NextFace)
		}
	}
}

FLinearColor AFaceManager::CheckFaceWin()
{
	for (int Row = 0; Row < 3; Row++)
	{
		int StartIndex = Row * 3;
		if (TilesArray[StartIndex]->BaseColor != FLinearColor::Gray &&
			TilesArray[StartIndex]->BaseColor == TilesArray[StartIndex + 1]->BaseColor &&
			TilesArray[StartIndex]->BaseColor == TilesArray[StartIndex + 2]->BaseColor)
		{
			return TilesArray[StartIndex]->BaseColor;
		}
	}

	for (int Col = 0; Col < 3; Col++)
	{
		if (TilesArray[Col]->BaseColor != FLinearColor::Gray &&
			TilesArray[Col]->BaseColor == TilesArray[Col+3]->BaseColor &&
			TilesArray[Col]->BaseColor == TilesArray[Col+6]->BaseColor)
		{
			return TilesArray[Col]->BaseColor;
		}
	}
	
	if (TilesArray[0]->BaseColor != FLinearColor::Gray && TilesArray[0]->BaseColor == TilesArray[4]->BaseColor && TilesArray[0]->BaseColor == TilesArray[8]->BaseColor)
	{
		return TilesArray[0]->BaseColor;
	}

	if (TilesArray[2]->BaseColor != FLinearColor::Gray && TilesArray[2]->BaseColor == TilesArray[4]->BaseColor && TilesArray[2]->BaseColor == TilesArray[6]->BaseColor)
	{
		return TilesArray[2]->BaseColor;
	}
	
	return FLinearColor::Gray;
}

bool AFaceManager::CheckFaceDraw()
{
	for(ATileActor* TileActor:TilesArray)
	{
		if(TileActor->BaseColor==FLinearColor::Gray)
		{
			return false;
		}
	}
	return true;
}

