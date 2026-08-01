# Interaction Action Pipeline Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** 인터랙션 시 캐릭터가 목표 위치로 이동 → 애니메이션 재생 → 실제 효과 실행되는 순차적 액션 파이프라인 구축

**Architecture:** `AGInteractableActor`가 `TArray<UGInteractionAction*>` 배열을 보유하고, `Interact()` 호출 시 `GInteractionComponent`의 파이프라인이 액션을 순서대로 실행한다. 각 액션은 완료 시 `FSimpleDelegate OnCompleted`를 호출해 다음 액션을 트리거한다.

**Tech Stack:** Unreal Engine 5, C++, AnimMontage, VInterpTo, FSimpleDelegate

## Global Constraints

- `!IsValid()` 금지 → `false == IsValid()` 사용
- `!Ptr` 금지 → `nullptr == Ptr` 사용
- 중괄호 항상 필수 — 한 줄 return도 반드시 `{}` 감싸기
- 기존 `UGInteractionCondition` 패턴 참고 (Async Load 방식 동일)

---

## File Map

| 파일 | 상태 | 역할 |
|---|---|---|
| `Source/ProjectG/Data/Interact/GInteractionAction.h/.cpp` | 신규 | 액션 베이스 클래스 + Finish() |
| `Source/ProjectG/Component/GInteractionActionPipeline.h/.cpp` | 신규 | 액션 순차 실행 관리 |
| `Source/ProjectG/Data/Interact/GInteractionAction_Execute.h/.cpp` | 신규 | InternalInteract() 호출 |
| `Source/ProjectG/Data/Interact/GInteractionAction_MoveTo.h/.cpp` | 신규 | 캐릭터 이동 (VInterpTo, Tick 기반) |
| `Source/ProjectG/Data/Interact/GInteractionAction_PlayMontage.h/.cpp` | 신규 | 몽타주 재생 + Notify/MontageEnd 대기 |
| `Source/ProjectG/Gimmick/GInteractableActor.h/.cpp` | 수정 | InteractPoint 추가, Interact() 변경, 액션 AsyncLoad |
| `Source/ProjectG/Component/GInteractionComponent.h/.cpp` | 수정 | 파이프라인 소유, Tick 전달, 입력 잠금 |

---

## Task 1: UGInteractionAction 베이스 클래스 + Pipeline

**Files:**
- Create: `Source/ProjectG/Data/Interact/GInteractionAction.h`
- Create: `Source/ProjectG/Data/Interact/GInteractionAction.cpp`
- Create: `Source/ProjectG/Component/GInteractionActionPipeline.h`
- Create: `Source/ProjectG/Component/GInteractionActionPipeline.cpp`

**Interfaces:**
- Produces:
  - `UGInteractionAction::Execute(AActor* OwnerActor, AActor* TargetActor)` — 순수 가상
  - `UGInteractionAction::Tick(float DeltaTime)` — 기본 빈 구현
  - `UGInteractionAction::Finish()` — OnCompleted 실행
  - `UGInteractionActionPipeline::Run(TArray<UGInteractionAction*>&, AActor*, AActor*, FSimpleDelegate)`
  - `UGInteractionActionPipeline::Tick(float DeltaTime)`
  - `UGInteractionActionPipeline::IsRunning() const`

- [ ] **Step 1: GInteractionAction.h 작성**

```cpp
// Source/ProjectG/Data/Interact/GInteractionAction.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GInteractionAction.generated.h"

UCLASS(Abstract)
class PROJECTG_API UGInteractionAction : public UObject
{
    GENERATED_BODY()

public:
    FSimpleDelegate OnCompleted;

    virtual void Execute(AActor* OwnerActor, AActor* TargetActor) PURE_VIRTUAL(UGInteractionAction::Execute, );
    virtual void Tick(float DeltaTime) {}

protected:
    void Finish();
};
```

- [ ] **Step 2: GInteractionAction.cpp 작성**

```cpp
// Source/ProjectG/Data/Interact/GInteractionAction.cpp
#include "Data/Interact/GInteractionAction.h"

void UGInteractionAction::Finish()
{
    OnCompleted.ExecuteIfBound();
}
```

- [ ] **Step 3: GInteractionActionPipeline.h 작성**

```cpp
// Source/ProjectG/Component/GInteractionActionPipeline.h
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GInteractionActionPipeline.generated.h"

class UGInteractionAction;

UCLASS()
class PROJECTG_API UGInteractionActionPipeline : public UObject
{
    GENERATED_BODY()

public:
    void Run(TArray<UGInteractionAction*>& InActions, AActor* InOwnerActor, AActor* InTargetActor, FSimpleDelegate InOnCompleted);
    void Tick(float DeltaTime);
    bool IsRunning() const { return bIsRunning; }

private:
    void RunNextAction();

private:
    UPROPERTY(Transient)
    TArray<TObjectPtr<UGInteractionAction>> Actions;

    UPROPERTY(Transient)
    TWeakObjectPtr<AActor> OwnerActorRef;

    UPROPERTY(Transient)
    TWeakObjectPtr<AActor> TargetActorRef;

    FSimpleDelegate OnAllCompleted;
    int32 CurrentIndex = 0;
    bool bIsRunning = false;
};
```

- [ ] **Step 4: GInteractionActionPipeline.cpp 작성**

```cpp
// Source/ProjectG/Component/GInteractionActionPipeline.cpp
#include "Component/GInteractionActionPipeline.h"
#include "Data/Interact/GInteractionAction.h"

void UGInteractionActionPipeline::Run(TArray<UGInteractionAction*>& InActions, AActor* InOwnerActor, AActor* InTargetActor, FSimpleDelegate InOnCompleted)
{
    Actions.Reset();
    for (UGInteractionAction* Action : InActions)
    {
        Actions.Add(Action);
    }

    OwnerActorRef = InOwnerActor;
    TargetActorRef = InTargetActor;
    OnAllCompleted = InOnCompleted;
    CurrentIndex = 0;
    bIsRunning = true;

    RunNextAction();
}

void UGInteractionActionPipeline::Tick(float DeltaTime)
{
    if (false == bIsRunning)
    {
        return;
    }

    int32 ActiveIndex = CurrentIndex - 1;
    if (ActiveIndex >= 0 && ActiveIndex < Actions.Num())
    {
        Actions[ActiveIndex]->Tick(DeltaTime);
    }
}

void UGInteractionActionPipeline::RunNextAction()
{
    if (CurrentIndex >= Actions.Num())
    {
        bIsRunning = false;
        OnAllCompleted.ExecuteIfBound();
        return;
    }

    UGInteractionAction* Action = Actions[CurrentIndex++];
    Action->OnCompleted.BindUObject(this, &ThisClass::RunNextAction);
    Action->Execute(OwnerActorRef.Get(), TargetActorRef.Get());
}
```

- [ ] **Step 5: 컴파일 확인**

에디터 또는 Visual Studio에서 빌드. 오류 없이 통과되어야 함.

- [ ] **Step 6: 커밋**

```
git add Source/ProjectG/Data/Interact/GInteractionAction.h
git add Source/ProjectG/Data/Interact/GInteractionAction.cpp
git commit -m "인터랙션 액션 베이스 클래스 추가"

git add Source/ProjectG/Component/GInteractionActionPipeline.h
git add Source/ProjectG/Component/GInteractionActionPipeline.cpp
git commit -m "인터랙션 액션 파이프라인 추가"
```

---

## Task 2: UGInteractionAction_Execute

**Files:**
- Create: `Source/ProjectG/Data/Interact/GInteractionAction_Execute.h`
- Create: `Source/ProjectG/Data/Interact/GInteractionAction_Execute.cpp`

**Interfaces:**
- Consumes: `UGInteractionAction::Execute`, `UGInteractionAction::Finish()`, `AGInteractableActor::InternalInteract(AActor*)`
- Produces: `UGInteractionAction_Execute` — OwnerActor의 InternalInteract 호출 후 즉시 Finish

- [ ] **Step 1: GInteractionAction_Execute.h 작성**

```cpp
// Source/ProjectG/Data/Interact/GInteractionAction_Execute.h
#pragma once

#include "CoreMinimal.h"
#include "Data/Interact/GInteractionAction.h"
#include "GInteractionAction_Execute.generated.h"

UCLASS()
class PROJECTG_API UGInteractionAction_Execute : public UGInteractionAction
{
    GENERATED_BODY()

public:
    virtual void Execute(AActor* OwnerActor, AActor* TargetActor) override;
};
```

- [ ] **Step 2: GInteractionAction_Execute.cpp 작성**

```cpp
// Source/ProjectG/Data/Interact/GInteractionAction_Execute.cpp
#include "Data/Interact/GInteractionAction_Execute.h"
#include "Gimmick/GInteractableActor.h"

void UGInteractionAction_Execute::Execute(AActor* OwnerActor, AActor* TargetActor)
{
    AGInteractableActor* Interactable = Cast<AGInteractableActor>(OwnerActor);
    if (IsValid(Interactable))
    {
        Interactable->InternalInteract(TargetActor);
    }

    Finish();
}
```

- [ ] **Step 3: 컴파일 확인**

- [ ] **Step 4: 커밋**

```
git add Source/ProjectG/Data/Interact/GInteractionAction_Execute.h
git add Source/ProjectG/Data/Interact/GInteractionAction_Execute.cpp
git commit -m "Execute 액션 추가 - InternalInteract 호출"
```

---

## Task 3: AGInteractableActor 수정

**Files:**
- Modify: `Source/ProjectG/Gimmick/GInteractableActor.h`
- Modify: `Source/ProjectG/Gimmick/GInteractableActor.cpp`

**Interfaces:**
- Consumes: `UGInteractionActionPipeline::Run(...)`, `UGInteractionComponent::RunActionPipeline(...)`
- Produces:
  - `AGInteractableActor::GetInteractPointTransform() const` — InteractPoint SceneComponent의 월드 Transform
  - `AGInteractableActor::GetInteractMontage() const` — 몽타주 반환 (nullptr 가능)
  - `AGInteractableActor::GetInteractNotifyName() const` — Notify 이름 반환
  - `AGInteractableActor::OnInteractionCompleted()` — 파이프라인 완료 콜백

- [ ] **Step 1: GInteractableActor.h 수정 — 프로퍼티 및 메서드 추가**

```cpp
// Source/ProjectG/Gimmick/GInteractableActor.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/GInteractable.h"
#include "GInteractableActor.generated.h"

class UGInteractionCondition;
class UGInteractionAction;
class USphereComponent;
class UAnimMontage;

UCLASS()
class PROJECTG_API AGInteractableActor : public AActor, public IGInteractable
{
    GENERATED_BODY()

public:
    AGInteractableActor();

    virtual FName GetID() const { return ID; }
    virtual int32 GetPriority() const override { return Priority; }
    virtual float GetHoldDuration() const override { return HoldDuration; }
    virtual EGInteractionState GetInteractionState(AActor* TargetActor) override;
    virtual void OnInteractStarted(AActor* TargetActor) override;
    virtual bool CanInteract(AActor* TargetActor) override;
    virtual void Interact(AActor* TargetActor) override;
    virtual void InternalInteract(AActor* TargetActor) override;

    FTransform GetInteractPointTransform() const;
    UAnimMontage* GetInteractMontage() const;
    FName GetInteractNotifyName() const { return InteractNotifyName; }

protected:
    virtual void BeginPlay() override;
    void RequestAsyncLoad();
    void OnConditionsLoaded();
    void RequestAsyncLoadActions();
    void OnActionsLoaded();
    void OnInteractionCompleted();

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Data")
    FName ID;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
    int32 Priority;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
    float HoldDuration;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction")
    TArray<TSoftClassPtr<UGInteractionCondition>> ConditionClassPtrs;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Actions")
    TArray<TSoftClassPtr<UGInteractionAction>> ActionClassPtrs;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Actions")
    TSoftObjectPtr<UAnimMontage> InteractMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Interaction|Actions")
    FName InteractNotifyName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
    TObjectPtr<UMeshComponent> MeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Interaction|Actions")
    TObjectPtr<USceneComponent> InteractPoint;

protected:
    UPROPERTY(Transient)
    EGInteractionState InteractionState;

    UPROPERTY(Transient)
    TArray<TObjectPtr<UGInteractionCondition>> Conditions;

    UPROPERTY(Transient)
    TArray<TObjectPtr<UGInteractionAction>> Actions;

    UPROPERTY(Transient)
    float StartTimestamp;
};
```

- [ ] **Step 2: GInteractableActor.cpp 수정**

```cpp
// Source/ProjectG/Gimmick/GInteractableActor.cpp
#include "Gimmick/GInteractableActor.h"

#include "Character/GCharacter.h"
#include "Component/GInteractionComponent.h"
#include "Data/GGameEnums.h"
#include "Data/Interact/GInteractionAction.h"
#include "Data/Interact/GInteractionCondition.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"

AGInteractableActor::AGInteractableActor()
    : Super()
    , InteractionState(EGInteractionState::Available)
{
    InteractPoint = CreateDefaultSubobject<USceneComponent>(TEXT("InteractPoint"));
}

EGInteractionState AGInteractableActor::GetInteractionState(AActor* TargetActor)
{
    if (InteractionState == EGInteractionState::Unavailable)
    {
        return InteractionState;
    }

    for (int32 Index = 0; Index < Conditions.Num(); Index++)
    {
        if (false == Conditions[Index]->IsSatisfied(TargetActor))
        {
            InteractionState = EGInteractionState::Pending;
            return InteractionState;
        }
    }

    InteractionState = EGInteractionState::Available;
    return InteractionState;
}

void AGInteractableActor::OnInteractStarted(AActor* TargetActor)
{
    UWorld* World = GetWorld();
    check(World);

    StartTimestamp = World->GetTimeSeconds();
}

bool AGInteractableActor::CanInteract(AActor* TargetActor)
{
    UWorld* World = GetWorld();
    check(World);

    float EndTimestamp = World->GetTimeSeconds();
    float Duration = (EndTimestamp - StartTimestamp);

    return Duration >= HoldDuration && GetInteractionState(TargetActor) == EGInteractionState::Available;
}

void AGInteractableActor::Interact(AActor* TargetActor)
{
    AGCharacter* Character = Cast<AGCharacter>(TargetActor);
    if (false == IsValid(Character))
    {
        return;
    }

    UGInteractionComponent* InteractionComp = Character->GetInteractionComponent();
    if (false == IsValid(InteractionComp))
    {
        return;
    }

    FSimpleDelegate OnCompleted = FSimpleDelegate::CreateUObject(this, &ThisClass::OnInteractionCompleted);
    InteractionComp->RunActionPipeline(Actions, this, TargetActor, OnCompleted);
}

void AGInteractableActor::InternalInteract(AActor* TargetActor)
{
    for (UGInteractionCondition* Condition : Conditions)
    {
        Condition->Apply(TargetActor);
    }
}

void AGInteractableActor::OnInteractionCompleted()
{
    InteractionState = EGInteractionState::Unavailable;
}

FTransform AGInteractableActor::GetInteractPointTransform() const
{
    if (false == IsValid(InteractPoint))
    {
        return GetActorTransform();
    }

    return InteractPoint->GetComponentTransform();
}

UAnimMontage* AGInteractableActor::GetInteractMontage() const
{
    return InteractMontage.Get();
}

void AGInteractableActor::BeginPlay()
{
    Super::BeginPlay();
    RequestAsyncLoad();
    RequestAsyncLoadActions();
}

void AGInteractableActor::RequestAsyncLoad()
{
    TArray<FSoftObjectPath> SoftPaths;
    for (const TSoftClassPtr<UGInteractionCondition>& ConditionPtr : ConditionClassPtrs)
    {
        SoftPaths.Add(ConditionPtr.ToSoftObjectPath());
    }

    if (SoftPaths.IsEmpty() || false == Conditions.IsEmpty())
    {
        return;
    }

    FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
    StreamableManager.RequestAsyncLoad(SoftPaths, FStreamableDelegate::CreateUObject(this, &ThisClass::OnConditionsLoaded));
}

void AGInteractableActor::OnConditionsLoaded()
{
    for (const TSoftClassPtr<UGInteractionCondition>& ConditionClassPtr : ConditionClassPtrs)
    {
        UClass* ConditionClass = ConditionClassPtr.Get();
        if (nullptr == ConditionClass)
        {
            continue;
        }

        UGInteractionCondition* NewCondition = NewObject<UGInteractionCondition>(this, ConditionClass);
        if (nullptr != NewCondition)
        {
            NewCondition->Init(this);
            Conditions.Add(NewCondition);
        }
    }
}

void AGInteractableActor::RequestAsyncLoadActions()
{
    TArray<FSoftObjectPath> SoftPaths;
    for (const TSoftClassPtr<UGInteractionAction>& ActionPtr : ActionClassPtrs)
    {
        SoftPaths.Add(ActionPtr.ToSoftObjectPath());
    }

    if (SoftPaths.IsEmpty() || false == Actions.IsEmpty())
    {
        return;
    }

    FStreamableManager& StreamableManager = UAssetManager::GetStreamableManager();
    StreamableManager.RequestAsyncLoad(SoftPaths, FStreamableDelegate::CreateUObject(this, &ThisClass::OnActionsLoaded));
}

void AGInteractableActor::OnActionsLoaded()
{
    for (const TSoftClassPtr<UGInteractionAction>& ActionClassPtr : ActionClassPtrs)
    {
        UClass* ActionClass = ActionClassPtr.Get();
        if (nullptr == ActionClass)
        {
            continue;
        }

        UGInteractionAction* NewAction = NewObject<UGInteractionAction>(this, ActionClass);
        if (nullptr != NewAction)
        {
            Actions.Add(NewAction);
        }
    }
}
```

- [ ] **Step 3: 컴파일 확인**

- [ ] **Step 4: 커밋**

```
git add Source/ProjectG/Gimmick/GInteractableActor.h
git add Source/ProjectG/Gimmick/GInteractableActor.cpp
git commit -m "GInteractableActor에 InteractPoint, 액션 파이프라인 연동 추가"
```

---

## Task 4: GInteractionComponent 수정 — 파이프라인 실행 + 입력 잠금

**Files:**
- Modify: `Source/ProjectG/Component/GInteractionComponent.h`
- Modify: `Source/ProjectG/Component/GInteractionComponent.cpp`

**Interfaces:**
- Consumes: `UGInteractionActionPipeline::Run(...)`, `UGInteractionActionPipeline::Tick(...)`
- Produces: `UGInteractionComponent::RunActionPipeline(TArray<UGInteractionAction*>&, AActor*, AActor*, FSimpleDelegate)`

- [ ] **Step 1: GInteractionComponent.h 수정 — Pipeline 추가**

```cpp
// Source/ProjectG/Component/GInteractionComponent.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GInteractionComponent.generated.h"

USTRUCT()
struct FGInteractionCandidate
{
    GENERATED_BODY()

public:
    UPROPERTY(Transient)
    TWeakObjectPtr<AActor> Actor;

    UPROPERTY(Transient)
    int32 Priority;

    UPROPERTY(Transient)
    float Score;
};

class ACharacter;
class USphereComponent;
class UGInteractionAction;
class UGInteractionActionPipeline;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTG_API UGInteractionComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UGInteractionComponent(const FObjectInitializer& ObjectInitializer);

    void OnInteractStarted();
    void OnInteractEnded();
    void Interact();

    void RunActionPipeline(TArray<UGInteractionAction*>& InActions, AActor* InOwnerActor, AActor* InTargetActor, FSimpleDelegate InOnCompleted);

protected:
    virtual void InitializeComponent() override;
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
    void UpdateFocusTarget();
    void UpdateCandidateScores();
    float CalculateScore(const TWeakObjectPtr<AActor>& Candidate);
    static bool CompareCandidates(const FGInteractionCandidate& ACandidate, const FGInteractionCandidate& BCandidate);

    void OnPipelineCompleted();

protected:
    UFUNCTION()
    void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Interaction)
    float ViewDotThreshold;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
    TObjectPtr<USphereComponent> InteractionSphereComponent;

protected:
    UPROPERTY(Transient)
    TWeakObjectPtr<ACharacter> CharacterRef;

    UPROPERTY(Transient)
    TArray<FGInteractionCandidate> InteractableCandidates;

    UPROPERTY(Transient)
    TWeakObjectPtr<AActor> InteractableActor;

    UPROPERTY(Transient)
    TObjectPtr<UGInteractionActionPipeline> ActionPipeline;
};
```

- [ ] **Step 2: GInteractionComponent.cpp 수정 — RunActionPipeline, Tick, 입력 잠금**

`RunActionPipeline`, `OnPipelineCompleted`, `TickComponent` 수정 부분만 추가:

```cpp
// 기존 include 아래에 추가
#include "Component/GInteractionActionPipeline.h"
#include "Data/Interact/GInteractionAction.h"
#include "GameFramework/PlayerController.h"
```

생성자에 파이프라인 초기화 추가:
```cpp
UGInteractionComponent::UGInteractionComponent(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
    , ViewDotThreshold(0.75f)
{
    InteractionSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphereComponent"));
    InteractionSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &UGInteractionComponent::OnSphereBeginOverlap);
    InteractionSphereComponent->OnComponentEndOverlap.AddDynamic(this, &UGInteractionComponent::OnSphereEndOverlap);

    ActionPipeline = CreateDefaultSubobject<UGInteractionActionPipeline>(TEXT("ActionPipeline"));

    bWantsInitializeComponent = true;
    PrimaryComponentTick.bCanEverTick = true;
}
```

TickComponent에 파이프라인 Tick 추가:
```cpp
void UGInteractionComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (IsValid(ActionPipeline) && ActionPipeline->IsRunning())
    {
        ActionPipeline->Tick(DeltaTime);
        return; // 파이프라인 실행 중에는 포커스 업데이트 생략
    }

    if (false == InteractableCandidates.IsEmpty())
    {
        UpdateFocusTarget();
    }
}
```

새 함수 추가:
```cpp
void UGInteractionComponent::RunActionPipeline(TArray<UGInteractionAction*>& InActions, AActor* InOwnerActor, AActor* InTargetActor, FSimpleDelegate InOnCompleted)
{
    if (false == IsValid(ActionPipeline))
    {
        return;
    }

    // 입력 잠금
    APlayerController* PC = Cast<APlayerController>(CharacterRef->GetController());
    if (nullptr != PC)
    {
        PC->SetIgnoreMoveInput(true);
    }

    FSimpleDelegate Combined = FSimpleDelegate::CreateLambda([this, InOnCompleted]()
    {
        OnPipelineCompleted();
        InOnCompleted.ExecuteIfBound();
    });

    ActionPipeline->Run(InActions, InOwnerActor, InTargetActor, Combined);
}

void UGInteractionComponent::OnPipelineCompleted()
{
    if (false == CharacterRef.IsValid())
    {
        return;
    }

    APlayerController* PC = Cast<APlayerController>(CharacterRef->GetController());
    if (nullptr != PC)
    {
        PC->ResetIgnoreMoveInput();
    }
}
```

- [ ] **Step 3: 컴파일 확인**

- [ ] **Step 4: 에디터에서 GKey 블루프린트 열어 ActionClassPtrs에 `UGInteractionAction_Execute` 추가 후 PIE 테스트**

기존 열쇠 획득이 정상 동작하면 통과.

- [ ] **Step 5: 커밋**

```
git add Source/ProjectG/Component/GInteractionComponent.h
git add Source/ProjectG/Component/GInteractionComponent.cpp
git commit -m "GInteractionComponent에 액션 파이프라인 실행 및 입력 잠금 추가"
```

---

## Task 5: UGInteractionAction_MoveTo

**Files:**
- Create: `Source/ProjectG/Data/Interact/GInteractionAction_MoveTo.h`
- Create: `Source/ProjectG/Data/Interact/GInteractionAction_MoveTo.cpp`

**Interfaces:**
- Consumes: `AGInteractableActor::GetInteractPointTransform()`, `UGInteractionAction::Finish()`
- Produces: `UGInteractionAction_MoveTo` — Tick마다 VInterpTo 이동, 도착 시 Finish

- [ ] **Step 1: GInteractionAction_MoveTo.h 작성**

```cpp
// Source/ProjectG/Data/Interact/GInteractionAction_MoveTo.h
#pragma once

#include "CoreMinimal.h"
#include "Data/Interact/GInteractionAction.h"
#include "GInteractionAction_MoveTo.generated.h"

class ACharacter;

UCLASS()
class PROJECTG_API UGInteractionAction_MoveTo : public UGInteractionAction
{
    GENERATED_BODY()

public:
    virtual void Execute(AActor* OwnerActor, AActor* TargetActor) override;
    virtual void Tick(float DeltaTime) override;

protected:
    UPROPERTY(EditDefaultsOnly, Category = "MoveTo")
    float MoveInterpSpeed = 5.f;

    UPROPERTY(EditDefaultsOnly, Category = "MoveTo")
    float RotationInterpSpeed = 10.f;

    UPROPERTY(EditDefaultsOnly, Category = "MoveTo")
    float ArrivalThreshold = 30.f;

private:
    UPROPERTY(Transient)
    TWeakObjectPtr<ACharacter> CharacterRef;

    FVector TargetLocation = FVector::ZeroVector;
    FRotator TargetRotation = FRotator::ZeroRotator;
};
```

- [ ] **Step 2: GInteractionAction_MoveTo.cpp 작성**

```cpp
// Source/ProjectG/Data/Interact/GInteractionAction_MoveTo.cpp
#include "Data/Interact/GInteractionAction_MoveTo.h"

#include "Gimmick/GInteractableActor.h"
#include "GameFramework/Character.h"

void UGInteractionAction_MoveTo::Execute(AActor* OwnerActor, AActor* TargetActor)
{
    CharacterRef = Cast<ACharacter>(TargetActor);
    if (false == CharacterRef.IsValid())
    {
        Finish();
        return;
    }

    AGInteractableActor* Interactable = Cast<AGInteractableActor>(OwnerActor);
    if (false == IsValid(Interactable))
    {
        Finish();
        return;
    }

    FTransform InteractTransform = Interactable->GetInteractPointTransform();
    TargetLocation = InteractTransform.GetLocation();
    TargetRotation = InteractTransform.Rotator();
}

void UGInteractionAction_MoveTo::Tick(float DeltaTime)
{
    if (false == CharacterRef.IsValid())
    {
        Finish();
        return;
    }

    FVector CurrentLocation = CharacterRef->GetActorLocation();
    FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, MoveInterpSpeed);
    CharacterRef->SetActorLocation(NewLocation);

    FRotator CurrentRotation = CharacterRef->GetActorRotation();
    FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationInterpSpeed);
    CharacterRef->SetActorRotation(NewRotation);

    float DistanceXY = FVector::DistXY(CurrentLocation, TargetLocation);
    if (DistanceXY <= ArrivalThreshold)
    {
        Finish();
    }
}
```

- [ ] **Step 3: 컴파일 확인**

- [ ] **Step 4: 에디터에서 테스트**

아이템 블루프린트의 `ActionClassPtrs`에 `GInteractionAction_MoveTo`, `GInteractionAction_Execute` 순서로 추가.
`InteractPoint` SceneComponent를 아이템 앞에 배치.
PIE에서 인터랙션 시 캐릭터가 해당 위치로 이동한 뒤 아이템 획득되면 통과.

- [ ] **Step 5: 커밋**

```
git add Source/ProjectG/Data/Interact/GInteractionAction_MoveTo.h
git add Source/ProjectG/Data/Interact/GInteractionAction_MoveTo.cpp
git commit -m "MoveTo 액션 추가 - 캐릭터 VInterpTo 이동"
```

---

## Task 6: UGInteractionAction_PlayMontage

**Files:**
- Create: `Source/ProjectG/Data/Interact/GInteractionAction_PlayMontage.h`
- Create: `Source/ProjectG/Data/Interact/GInteractionAction_PlayMontage.cpp`

**Interfaces:**
- Consumes: `AGInteractableActor::GetInteractMontage()`, `AGInteractableActor::GetInteractNotifyName()`, `UGInteractionAction::Finish()`
- Produces: `UGInteractionAction_PlayMontage` — 몽타주 재생, Notify 또는 MontageEnd 시 Finish

- [ ] **Step 1: GInteractionAction_PlayMontage.h 작성**

```cpp
// Source/ProjectG/Data/Interact/GInteractionAction_PlayMontage.h
#pragma once

#include "CoreMinimal.h"
#include "Data/Interact/GInteractionAction.h"
#include "GInteractionAction_PlayMontage.generated.h"

class UAnimInstance;
class ACharacter;

UCLASS()
class PROJECTG_API UGInteractionAction_PlayMontage : public UGInteractionAction
{
    GENERATED_BODY()

public:
    virtual void Execute(AActor* OwnerActor, AActor* TargetActor) override;

private:
    UFUNCTION()
    void OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload);

    UFUNCTION()
    void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);

    void UnbindDelegates();

private:
    UPROPERTY(Transient)
    TWeakObjectPtr<UAnimInstance> AnimInstanceRef;

    UPROPERTY(Transient)
    TWeakObjectPtr<UAnimMontage> MontageRef;

    FName WaitNotifyName = NAME_None;
};
```

- [ ] **Step 2: GInteractionAction_PlayMontage.cpp 작성**

```cpp
// Source/ProjectG/Data/Interact/GInteractionAction_PlayMontage.cpp
#include "Data/Interact/GInteractionAction_PlayMontage.h"

#include "Animation/AnimInstance.h"
#include "Gimmick/GInteractableActor.h"
#include "GameFramework/Character.h"

void UGInteractionAction_PlayMontage::Execute(AActor* OwnerActor, AActor* TargetActor)
{
    ACharacter* Character = Cast<ACharacter>(TargetActor);
    if (false == IsValid(Character))
    {
        Finish();
        return;
    }

    AGInteractableActor* Interactable = Cast<AGInteractableActor>(OwnerActor);
    if (false == IsValid(Interactable))
    {
        Finish();
        return;
    }

    UAnimMontage* Montage = Interactable->GetInteractMontage();
    if (false == IsValid(Montage))
    {
        Finish();
        return;
    }

    UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
    if (false == IsValid(AnimInstance))
    {
        Finish();
        return;
    }

    AnimInstanceRef = AnimInstance;
    MontageRef = Montage;
    WaitNotifyName = Interactable->GetInteractNotifyName();

    Character->PlayAnimMontage(Montage);

    if (false == WaitNotifyName.IsNone())
    {
        AnimInstance->OnPlayMontageNotifyBegin.AddDynamic(this, &ThisClass::OnNotifyBegin);
    }
    else
    {
        AnimInstance->OnMontageEnded.AddDynamic(this, &ThisClass::OnMontageEnded);
    }
}

void UGInteractionAction_PlayMontage::OnNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointPayload)
{
    if (NotifyName != WaitNotifyName)
    {
        return;
    }

    UnbindDelegates();
    Finish();
}

void UGInteractionAction_PlayMontage::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    if (Montage != MontageRef.Get())
    {
        return;
    }

    UnbindDelegates();
    Finish();
}

void UGInteractionAction_PlayMontage::UnbindDelegates()
{
    if (false == AnimInstanceRef.IsValid())
    {
        return;
    }

    AnimInstanceRef->OnPlayMontageNotifyBegin.RemoveDynamic(this, &ThisClass::OnNotifyBegin);
    AnimInstanceRef->OnMontageEnded.RemoveDynamic(this, &ThisClass::OnMontageEnded);
}
```

- [ ] **Step 3: 컴파일 확인**

- [ ] **Step 4: 에디터에서 테스트**

아이템 블루프린트에 `InteractMontage` 설정.
`ActionClassPtrs`: `[MoveTo, PlayMontage, Execute]` 순서로 구성.

테스트 케이스:
1. `InteractNotifyName` 비어있을 때 → 몽타주 끝나면 아이템 획득
2. `InteractNotifyName` 지정 시 → Notify 발생 시점에 아이템 획득
3. `InteractMontage` 없을 때 → PlayMontage 액션이 즉시 skip되어 Execute 실행

- [ ] **Step 5: 커밋**

```
git add Source/ProjectG/Data/Interact/GInteractionAction_PlayMontage.h
git add Source/ProjectG/Data/Interact/GInteractionAction_PlayMontage.cpp
git commit -m "PlayMontage 액션 추가 - Notify/MontageEnd 기반 타이밍 처리"
```
