// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Npc.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "MyPlayer.h"
#include "MyGameInstance.h"
#include "UIManager.h"
#include "DataManager.h"
#include "Popup/NpcWidget.h"
#include "Components/WidgetComponent.h"
#include "Component/NpcInfoWidget.h"


// Sets default values
ANpc::ANpc()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    SetInfoWidgetComponent();
    SetRangeBox();
}

// Called when the game starts or when spawned
void ANpc::BeginPlay()
{
	Super::BeginPlay();
    NpcInfo->SetVisibility(false);
	GInstance = Cast<UMyGameInstance>(GetWorld()->GetGameInstance());

    BindOverlapFunctions();
    //Init();
}

// Called every frame
void ANpc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANpc::Init()
{
    FString BlueprintClassName = GetClass()->GetName();
    FString IndexString = BlueprintClassName.Replace(TEXT("BP_Npc_"), TEXT(""));
    int32 Index = FCString::Atoi(*IndexString);
    Id = Index;
    UE_LOG(LogTemp, Warning, TEXT("Npc Init! %d"), Index);
    MyData = (GInstance->GetDataMgr()->GetNpcData(Index));
    GInstance->Npcs.Add(Index, this);

    if (NpcInfo->GetWidget() == nullptr)
    {
        return;
    }
    if (auto InfoWidget = Cast<UNpcInfoWidget>(NpcInfo->GetWidget()))
    {
        InfoWidget->Init(MyData);
    }
    UpdateQuestMark();
}

void ANpc::Interact()
{
    TObjectPtr<UNpcWidget> NpcWidget = Cast<UNpcWidget>(GInstance->GetUIMgr()->PopupUI("Npc"));
    if(NpcWidget != nullptr)
    {
        NpcWidget->Init(this, GInstance);
    }

    if (TargetQuest.IsEmpty() == false)
    {
        int32 QuestId = *TargetQuest.Peek();

        NpcWidget->SetDialogueMode(QuestId, 2);
    }

    // 만약 플레이어가 이 Npc에 대한 퀘스를 가지고 있다면
    // 1. 패킷 전송

}

void ANpc::AddQuest(int32 QuestId)
{
    // 메인 퀘스트라면 MainQuest에
    Protocol::EQuestGrade Grade = static_cast<Protocol::EQuestGrade>(GInstance->GetDataMgr()->GetQuestData(QuestId)->QuestGrade);

    if (Grade == Protocol::QUEST_MAIN)
    {
        MainQuest.Add(QuestId);
    }

    if (Grade == Protocol::QUEST_NORMAL)
    {
        NormalQuest.Add(QuestId);
    }

    UpdateQuestMark();
    // 일반 퀘스트라면 Normal 퀘스트에 추가
}

void ANpc::AddCompletableQuest(int32 QuestId)
{
    Protocol::EQuestGrade Grade = static_cast<Protocol::EQuestGrade>(GInstance->GetDataMgr()->GetQuestData(QuestId)->QuestGrade);

    if (Grade == Protocol::QUEST_MAIN)
    {
        CompletableMainQuests.Add(QuestId);
    }

    if (Grade == Protocol::QUEST_NORMAL)
    {
        CompletableNormalQuests.Add(QuestId);
    }

    UpdateQuestMark();
}

void ANpc::AddTargetQuest(int32 QuestId)
{
    TargetQuest.Enqueue(QuestId);
    UpdateQuestMark();
}

void ANpc::RemoveQuest(int32 QuestId)
{
    Protocol::EQuestGrade Grade = static_cast<Protocol::EQuestGrade>(GInstance->GetDataMgr()->GetQuestData(QuestId)->QuestGrade);

    if (Grade == Protocol::QUEST_MAIN)
    {
        MainQuest.Remove(QuestId);
    }

    if (Grade == Protocol::QUEST_NORMAL)
    {
        NormalQuest.Remove(QuestId);
    }
    UpdateQuestMark();
}

void ANpc::RemoveCompletableQuest(int32 QuestId)
{
    Protocol::EQuestGrade Grade = static_cast<Protocol::EQuestGrade>(GInstance->GetDataMgr()->GetQuestData(QuestId)->QuestGrade);

    if (Grade == Protocol::QUEST_MAIN)
    {
        CompletableMainQuests.Remove(QuestId);
    }

    if (Grade == Protocol::QUEST_NORMAL)
    {
        CompletableNormalQuests.Remove(QuestId);
    }
    UpdateQuestMark();
}

void ANpc::RemoveTargetQuest()
{
    TargetQuest.Pop();
    UpdateQuestMark();
}

#pragma region OverlapFunctions
void ANpc::OnOInteractverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UE_LOG(LogTemp, Warning, TEXT("Interact Overlap Begin with Not MyPlayer %s"), *OtherActor->GetName());
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        if (OtherComp->GetCollisionProfileName() == FName("MyPlayer"))
        {
            UE_LOG(LogTemp, Warning, TEXT("Interact Overlap Begin with %s"), *OtherActor->GetName());
            Cast<AMyPlayer>(OtherActor)->SetInteractingNpc(this);
        }
    }
}

void ANpc::OnInteractOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        if (OtherComp->GetCollisionProfileName() == FName("MyPlayer"))
        {
            UE_LOG(LogTemp, Warning, TEXT("Interact Overlap End with %s"), *OtherActor->GetName());
            Cast<AMyPlayer>(OtherActor)->SetInteractingNpc(nullptr);
        }
    }
}

void ANpc::OnDetectOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        if (OtherComp->GetCollisionProfileName() == FName("MyPlayer"))
        {
            UE_LOG(LogTemp, Warning, TEXT("Detect Overlap Begin with %s"), *OtherActor->GetName());
            NpcInfo->SetVisibility(true);
        }
    }
}

void ANpc::OnDetectOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && (OtherActor != this) && OtherComp)
    {
        if (OtherComp->GetCollisionProfileName() == FName("MyPlayer"))
        {
            UE_LOG(LogTemp, Warning, TEXT("Detect Overlap End with %s"), *OtherActor->GetName());
            NpcInfo->SetVisibility(false);
        }
    }
}
#pragma endregion

/*
    ==================== Private ===================
*/
void ANpc::UpdateQuestMark()
{
    // 0. 본인이 타겟이 되는 퀘스트가 있다면 먼저 알림

    // 1. 완료 가능 퀘스트가 있다면 먼저 알림
    if (TargetQuest.IsEmpty() == false)
    {
        Cast<UNpcInfoWidget>(NpcInfo->GetWidget())->DrawTargetMark();
    }
    else if (CompletableMainQuests.Num() > 0)
    {
        Cast<UNpcInfoWidget>(NpcInfo->GetWidget())->DrawCompletableMark(Protocol::QUEST_MAIN);
    }
    else if (CompletableNormalQuests.Num() > 0)
    {
        Cast<UNpcInfoWidget>(NpcInfo->GetWidget())->DrawCompletableMark(Protocol::QUEST_NORMAL);
    }
    else if (MainQuest.Num() > 0)
    {
        // 2. 메인 퀘스트가 있다면 메인 퀘스트 먼저 알림
        Cast<UNpcInfoWidget>(NpcInfo->GetWidget())->DrawQuestMark(Protocol::QUEST_MAIN);
    }
    else if (NormalQuest.Num() > 0)
    {
        // 3. 없다면 일반 퀘스트 알림
        Cast<UNpcInfoWidget>(NpcInfo->GetWidget())->DrawQuestMark(Protocol::QUEST_NORMAL);
    }
    else
    {
        // 4. 다 없다면 그냥 끔
        Cast<UNpcInfoWidget>(NpcInfo->GetWidget())->RemoveQuestMark();
        Cast<UNpcInfoWidget>(NpcInfo->GetWidget())->RemoveCompletableMark();
        Cast<UNpcInfoWidget>(NpcInfo->GetWidget())->RemoveTargetMark();
    }
}
void ANpc::SetInfoWidgetComponent()
{
    NpcInfo = CreateDefaultSubobject<UWidgetComponent>(TEXT("NpcInfo"));
    NpcInfo->SetDrawSize(FVector2D(60.f, 5.f));
    NpcInfo->SetupAttachment(RootComponent);

}
void ANpc::SetRangeBox()
{
    InteractRange = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractRange"));
    InteractRange->InitBoxExtent(FVector(100.0f, 100.0f, 100.0f));
    InteractRange->SetCollisionProfileName(TEXT("InteractRange"));
    InteractRange->SetupAttachment(RootComponent);
    InteractRange->SetGenerateOverlapEvents(true);

    DetectionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DetectRange"));
    DetectionBox->InitBoxExtent(FVector(1000.0f, 1000.0f, 500.0f));
    DetectionBox->SetCollisionProfileName(TEXT("InteractRange"));
    DetectionBox->SetupAttachment(RootComponent);
    DetectionBox->SetGenerateOverlapEvents(true);
}
void ANpc::BindOverlapFunctions()
{
    InteractRange->OnComponentBeginOverlap.AddDynamic(this, &ANpc::OnOInteractverlapBegin);
    InteractRange->OnComponentEndOverlap.AddDynamic(this, &ANpc::OnInteractOverlapEnd);

    DetectionBox->OnComponentBeginOverlap.AddDynamic(this, &ANpc::OnDetectOverlapBegin);
    DetectionBox->OnComponentEndOverlap.AddDynamic(this, &ANpc::OnDetectOverlapEnd);
}




