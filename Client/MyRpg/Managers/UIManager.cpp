// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/UIManager.h"
#include "MyGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "DataManager.h"
#include "MyPlayerController.h"
#include "Popup/PopupWidget.h"
#include "Popup/AlarmWidget.h"
#include "DynamicInfo/DynamicInfoWidget.h"
#include "Scene/SceneWidget.h"
#include "Scene/InGameMainWidget.h"

#include "Kismet/GameplayStatics.h"
#include "MyGameModeBase.h"


UUIManager::UUIManager()
{
}

void UUIManager::Init(TObjectPtr<UMyGameInstance> Instance)
{
    GInstance = Instance;
    
}

void UUIManager::InitializeSceneWidget()
{
    if (SceneWidget == nullptr)
    {
        return;
    }

    SceneWidget->Init(GInstance.Get());
}

void UUIManager::Alarm(FString Text)
{
    auto Widget = Cast<UAlarmWidget>(PopupUI(TEXT("Alarm")));
    Widget->SetText(Text);
}


void UUIManager::SetMainWidget(TSubclassOf<class UUserWidget> WidgetClass)
{
    if (SceneWidget.IsNull() == false)
    {
        SceneWidget->RemoveFromParent();
    }
    SceneWidget = CreateWidget<USceneWidget>(GInstance->GetWorld(), WidgetClass);
    SceneWidget->AddToViewport();
}

TObjectPtr<UUserWidget> UUIManager::PopupUI(FString Type)
{
    if (PopupUIList.Contains(Type))
    {
        CloseUI(Type);
        return nullptr;
    }

	FString Path = GInstance->GetDataMgr()->GetUIPath(Type);
	UClass* PopupWidgetClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr, *Path);
        // 위젯 인스턴스 생성
    TObjectPtr<UUserWidget> PopupWidget = CreateWidget<UUserWidget>(GInstance->GetWorld(), PopupWidgetClass);
    if(PopupWidget == nullptr)
        return nullptr;

    PopupWidget->AddToViewport();
    auto Popup = Cast<UPopupWidget>(PopupWidget);
    if(Popup != nullptr)
    {
        PopupUIList.Add(Type, Popup);
    }
    if (Type == TEXT("Npc"))
    {
        SceneWidget->SetVisibility(ESlateVisibility::Hidden);
    }

    if (PopupUIList.Num() == 1)
    {
        SetPlayerMouseCursor(true);
    }

    return PopupWidget;

}

TObjectPtr<class UUserWidget> UUIManager::PopupDynamicInfoUI(FString Type)
{
    FString Path = GInstance->GetDataMgr()->GetUIPath(Type);
    UClass* PopupWidgetClass = StaticLoadClass(UUserWidget::StaticClass(), nullptr, *Path);
    // 위젯 인스턴스 생성
    TObjectPtr<UUserWidget> Widget = CreateWidget<UUserWidget>(GInstance->GetWorld(), PopupWidgetClass);
    if (Widget == nullptr)
        return nullptr;

    Widget->AddToViewport();
    auto Popup = Cast<UDynamicInfoWidget>(Widget);
    if (Popup != nullptr)
    {
        DynamicInfoUIList.Add(Type, Popup);
    }

    return Widget;
}

TObjectPtr<UUserWidget> UUIManager::CreateSubItem(TSubclassOf<class UUserWidget> SubItem)
{
    TObjectPtr<UUserWidget> Widget = CreateWidget<UUserWidget>(GInstance->GetWorld(), SubItem);
    if (Widget == nullptr)
        return nullptr;
    Widget->AddToViewport();
    
    return Widget;
}

void UUIManager::RefreshUI(FString UiType)
{
    if (PopupUIList.Contains(UiType) == false)
    {
        return;
    }
    PopupUIList[UiType]->Refresh();
}

void UUIManager::CloseUI(FString Type)
{
    if (PopupUIList.Contains(Type) != false)
    {
        TObjectPtr<UUserWidget> Widget = PopupUIList[Type];

        if (Widget != nullptr) // Null 체크 추가
        {
            Widget->RemoveFromViewport();
            PopupUIList.Remove(Type);
            if (Type == TEXT("Npc"))
            {
                SceneWidget->SetVisibility(ESlateVisibility::Visible);
            }
        }

        if (PopupUIList.Num() == 0)
        {
            SetPlayerMouseCursor(false);
        }
    }

    if (DynamicInfoUIList.Contains(Type) != false)
    {
        TObjectPtr<UUserWidget> Widget = DynamicInfoUIList[Type];

        if (Widget != nullptr) // Null 체크 추가
        {
            Widget->RemoveFromViewport();
            PopupUIList.Remove(Type);
        }
    }

    
   
}

void UUIManager::CloseAllUI()
{
    TArray<FString> Keys;
    PopupUIList.GenerateKeyArray(Keys);
    for (auto& Key : Keys)
    {
        CloseUI(Key);
    }
}

TObjectPtr<class UPopupWidget> UUIManager::GetPopupUI(FString Type)
{
    if (PopupUIList.Contains(Type) == false)
    {
        return nullptr;
    }
    return PopupUIList[Type];
}

TObjectPtr<class UUserWidget> UUIManager::GetDynamicUI(FString Type)
{
    if (DynamicInfoUIList.Contains(Type) == false)
    {
        return nullptr;
    }
    return DynamicInfoUIList[Type];
}

void UUIManager::SetPlayerMouseCursor(bool Value)
{
    TObjectPtr<AMyPlayerController> PC = Cast<AMyPlayerController>(GInstance->GetWorld()->GetFirstPlayerController());
    PC->SetShowMouseCursor(Value);
    PC->SetIgnoreLookInput(Value);
    PC->SetIgnoreMoveInput(Value);
    PC->StopMove();
    
    if(Value == true)
    {
        PC->SetInputMode(FInputModeUIOnly());
    }
    else
    {
        PC->SetInputMode(FInputModeGameOnly());
    }
}
