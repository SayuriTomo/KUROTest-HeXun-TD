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
	
	for(ATileActor* Tile:TilesArray)
	{
		Tile->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);
	}
}

// Called every frame
void AFaceManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	for(int i = 0;i<TilesArray.Num();i++)
	{
		if(TilesArray[i]->bIsSelected)
		{
			/*
			NextFace.Empty();
			if(i<3)
			{
				NextFace.Add(ENextFace::FACE_UP);
				UE_LOG(LogTemp,Warning,TEXT("UP"))
			}
			else if(i>5)
			{
				NextFace.Add(ENextFace::FACE_DOWN);
				UE_LOG(LogTemp,Warning,TEXT("D"))
			}

			if(i==0||i==3||i==6)
			{
				NextFace.Add(ENextFace::FACE_LEFT);
				UE_LOG(LogTemp,Warning,TEXT("L"))
			}
			else if(i==2||i==5||i==8)
			{
				NextFace.Add(ENextFace::FACE_RIGHT);
				UE_LOG(LogTemp,Warning,TEXT("R"))
			}

			if(i==4)
			{
				NextFace.Add(ENextFace::FACE_OPPOSITE);
				UE_LOG(LogTemp,Warning,TEXT("OPPO"))
			}
			*/
			
			TilesArray[i]->bIsSelected = false;
			NextFace = TilesArray[i]->NextFace[0];
			bIsActivating = true;
			break;
		}
	}
}

