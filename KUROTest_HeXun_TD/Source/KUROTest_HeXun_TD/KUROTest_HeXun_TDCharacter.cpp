// Copyright Epic Games, Inc. All Rights Reserved.

#include "KUROTest_HeXun_TDCharacter.h"
#include "KUROTest_HeXun_TDProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TileActor.h"
#include "TileManager.h"


//////////////////////////////////////////////////////////////////////////
// AKUROTest_HeXun_TDCharacter

AKUROTest_HeXun_TDCharacter::AKUROTest_HeXun_TDCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

}

void AKUROTest_HeXun_TDCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
		PlayerController->SetShowMouseCursor(true);
		//PlayerController->SetIgnoreLookInput(true);
	}
}

void AKUROTest_HeXun_TDCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CallTrace();
}

void AKUROTest_HeXun_TDCharacter::Select()
{
	if(bIsMyTurn)
	{
		if(LastTileWatching&&LastTileWatching->BaseColor==FLinearColor::Gray)
		{
			bIsMyTurn = false;
			LastTileWatching->bIsSelected = true;
			LastTileWatching->BaseColor = FLinearColor::Blue;
			LastTileWatching->ChangeColor(FLinearColor::Blue,true);
			LastTileWatching = nullptr;
		}
	}
}

void AKUROTest_HeXun_TDCharacter::CallTrace()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		FVector MousePosition, MouseRotation;
		if(PlayerController->DeprojectMousePositionToWorld(
			MousePosition,MouseRotation))
		{
			FCollisionQueryParams CollisionParams(TEXT("CollisionParams"),true,NULL);
			CollisionParams.bTraceComplex = true;
			CollisionParams.bReturnPhysicalMaterial=false;
			CollisionParams.AddIgnoredActor(this);

			FVector EndLocation = MousePosition+MouseRotation*1000;
			FHitResult HitResult;

			if(GetWorld()->LineTraceSingleByChannel(HitResult,MousePosition,EndLocation,ECC_Visibility,CollisionParams))
			{
				ProcessTraceResult(HitResult);
			}
			
			//DrawDebugLine(GetWorld(), MousePosition, HitResult.GetActor() ? HitResult.Location : EndLocation, FColor::Red, false, 1.0f);
		}
	}
}

void AKUROTest_HeXun_TDCharacter::ProcessTraceResult(FHitResult& HitResult)
{
	if(ATileActor* TileActor = Cast<ATileActor>(HitResult.GetActor()))
	{
		
		if(LastTileWatching)
		{
			if(LastTileWatching!=TileActor)
			{
				LastTileWatching->bIsWatched = false;
				LastTileWatching = TileActor;
				LastTileWatching->bIsWatched = true;
			}
		}
		else
		{
			LastTileWatching = TileActor;
			LastTileWatching->bIsWatched = true;
		}
	}
	else
	{
		if(LastTileWatching)
		{
			LastTileWatching->bIsWatched = false;
			LastTileWatching = nullptr;
		}
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AKUROTest_HeXun_TDCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKUROTest_HeXun_TDCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AKUROTest_HeXun_TDCharacter::Look);

		//Select
		EnhancedInputComponent->BindAction(SelectAction, ETriggerEvent::Triggered, this, &AKUROTest_HeXun_TDCharacter::Select);

	}
}


void AKUROTest_HeXun_TDCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AKUROTest_HeXun_TDCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AKUROTest_HeXun_TDCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AKUROTest_HeXun_TDCharacter::GetHasRifle()
{
	return bHasRifle;
}