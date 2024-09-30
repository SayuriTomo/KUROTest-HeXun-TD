// Fill out your copyright notice in the Description page of Project Settings.


#include "KUROTest_HeXun_TD/Public/TileManager.h"

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
			Face->bIsActivating = false;
			if(Face->NextFace.Num()==1)
			{
				switch(Face->NextFace[0])
				{
					case FACE_UP:
						AddActorLocalRotation(FRotator(90,0,0));
						break;

					case FACE_DOWN:
						AddActorLocalRotation(FRotator(-90,0,0));
						break;
					
					case FACE_LEFT:
						AddActorLocalRotation(FRotator(0,90,0));
						break;

					case FACE_RIGHT:
						AddActorLocalRotation(FRotator(0,-90,0));
						break;

					case FACE_OPPOSITE:
						AddActorLocalRotation(FRotator(0,180,180));
						break;

					default:
						break;
				}
			}
		}
	}
}