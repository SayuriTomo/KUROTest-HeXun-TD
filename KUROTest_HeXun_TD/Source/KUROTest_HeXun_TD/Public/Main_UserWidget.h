// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TileManager.h"

#include "Blueprint/UserWidget.h"
#include "Main_UserWidget.generated.h"

/**
 * 
 */
UCLASS()
class KUROTEST_HEXUN_TD_API UMain_UserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(BlueprintReadWrite) ATileManager* TileManager;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))class UTextBlock* TurnBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))class UTextBlock* LastWinnerBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))class UTextBlock* PlayerScoreBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))class UTextBlock* EnemyScoreBlock;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))class UButton* StartButton;
};
