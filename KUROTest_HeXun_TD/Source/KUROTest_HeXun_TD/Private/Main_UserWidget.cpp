// Fill out your copyright notice in the Description page of Project Settings.


#include "Main_UserWidget.h"

#include <string>

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "KUROTest_HeXun_TD/KUROTest_HeXun_TDCharacter.h"

void UMain_UserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATileManager::StaticClass(), OutActors);
	for(AActor* OutActor : OutActors)
	{
		if(Cast<ATileManager>(OutActor))
		{
			TileManager = Cast<ATileManager>(OutActor);
			break;
		}
	}
}

void UMain_UserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(Cast<AKUROTest_HeXun_TDCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn())->bIsMyTurn)
	{
		TurnBlock->SetText(FText::FromString(TEXT("你的回合")));
		TurnBlock->SetColorAndOpacity(FLinearColor::Blue);
	}
	else
	{
		TurnBlock->SetText(FText::FromString(TEXT("敌人回合")));
		TurnBlock->SetColorAndOpacity(FLinearColor::Red);
	}
	if(TileManager)
	{
		if(!TileManager->bIsGameStart)
		{
			StartButton->SetVisibility(ESlateVisibility::Visible);
			PlayerScoreBlock->SetVisibility(ESlateVisibility::Hidden);
			EnemyScoreBlock->SetVisibility(ESlateVisibility::Hidden);
			TurnBlock->SetVisibility(ESlateVisibility::Hidden);
		}
		else
		{
			StartButton->SetVisibility(ESlateVisibility::Hidden);
			PlayerScoreBlock -> SetVisibility(ESlateVisibility::Visible);
			EnemyScoreBlock->SetVisibility(ESlateVisibility::Visible);
			TurnBlock->SetVisibility(ESlateVisibility::Visible);
		}
		PlayerScoreBlock->SetText(FText::FromString(FString::FromInt(TileManager->PlayerScore)));
		EnemyScoreBlock->SetText(FText::FromString(FString::FromInt(TileManager->EnemyScore)));

		switch (TileManager->LastWinner)
		{
			case 0:
				LastWinnerBlock->SetText(FText::FromString(TEXT("无")));
				break;
			case 1:
				LastWinnerBlock->SetText(FText::FromString(TEXT("玩家")));
				break;
			case 2:
				LastWinnerBlock->SetText(FText::FromString(TEXT("AI敌人")));
				break;
			default:
				break;
		}
	}
}
