// Fill out your copyright notice in the Description page of Project Settings.


#include "Managers/SceneManager.h"
#include "MyGameInstance.h"
#include "SoundManager.h"
#include "DataManager.h"
#include "UIManager.h"
#include <Kismet/GameplayStatics.h>

void USceneManager::Init(TObjectPtr<class UMyGameInstance> Instance)
{
    GInstance = Instance;
}

void USceneManager::LoadSceneAsync(ESceneType Type)
{
   
    ESceneType PrevType = CurrentType;
    CurrentType = Type;
    FName LevelName(*GInstance->GetDataMgr()->GetScenePath(CurrentType));

    FLatentActionInfo LatentInfo;
    LatentInfo.CallbackTarget = this; // 콜백 타겟
    LatentInfo.UUID = 0; // 고유 ID 설정
    LatentInfo.Linkage = 0; // 연결 설정
    LatentInfo.ExecutionFunction = FName("OnLoadedScene");
    UGameplayStatics::LoadStreamLevel(
        GInstance->GetWorld(),
        LevelName,
        true, // SetVisible
        true, // MakeVisibleAfterLoad
        LatentInfo // LatentActionInfo for callback
    );
    if (PrevType == ESceneType::START)
    {
        return;
    }

    FLatentActionInfo LatentInfo2;
    LatentInfo2.CallbackTarget = this; // 콜백 타겟
    LatentInfo2.UUID = 1; // 고유 ID 설정
    LatentInfo2.Linkage = 0; // 연결 설정

    FName PrevName(*GInstance->GetDataMgr()->GetScenePath(PrevType));
    UGameplayStatics::UnloadStreamLevel(GInstance->GetWorld(), PrevName, LatentInfo2, true // MakeVisibleAfterLoad
    );

   //GInstance->GetUIMgr()->SceneWid
}

void USceneManager::OnLoadedScene()
{
    UE_LOG(LogTemp, Warning, TEXT("On Loaded Scene"));
    GInstance->GetSoundMgr()->PlayBgm(CurrentType);
}
