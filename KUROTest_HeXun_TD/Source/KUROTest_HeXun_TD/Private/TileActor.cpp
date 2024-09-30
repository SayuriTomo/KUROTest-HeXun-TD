// Fill out your copyright notice in the Description page of Project Settings.


#include "KUROTest_HeXun_TD/Public/TileActor.h"

#include "StaticMeshAttributes.h"

// Sets default values
ATileActor::ATileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tile Mesh"));
	SetRootComponent(TileMesh);
}

// Called when the game starts or when spawned
void ATileActor::BeginPlay()
{
	Super::BeginPlay();

	// Create Material Instance
	if(MaterialClass)
	{
		MaterialInstance = UMaterialInstanceDynamic::Create(MaterialClass, this);
		TileMesh->SetMaterial(0,MaterialInstance);
	}
	ChangeColor(FLinearColor::Gray,true);
}

// Called every frame
void ATileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsWatched)
	{
		ChangeColor(FLinearColor::Blue,false);
	}
	else
	{
		ChangeColor(FLinearColor::Gray,false);
	}
}

void ATileActor::ChangeColor(FLinearColor ColorToChange, bool bIsBaseOrEdge)
{
	if(MaterialInstance)
	{
		if(bIsBaseOrEdge)
		{
			MaterialInstance->SetVectorParameterValue("BaseColor", ColorToChange);
		}
		else
		{
			MaterialInstance->SetVectorParameterValue("EdgeColor", ColorToChange);
		}
		
	}
}

