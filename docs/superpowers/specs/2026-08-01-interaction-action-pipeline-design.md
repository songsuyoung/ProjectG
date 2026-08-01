# Interaction Action Pipeline 설계

**날짜:** 2026-08-01  
**목표:** Lies of P 스타일 아이템 인터랙션 — 캐릭터 이동 + 애니메이션 + 실제 효과를 순차적으로 실행하는 확장 가능한 구조

---

## 인터랙션 플로우

```
[E 홀드 → CanInteract() 통과]
        ↓
  Interact() 호출
        ↓
  ActionPipeline.Run(Actions[])
        ↓
  Action[0].Execute() → Finish() → Action[1].Execute() → Finish() → ...
        ↓
  모든 액션 완료
```

---

## 클래스 구조

### 1. `UGInteractionAction` (신규, 베이스)

```
UObject
└── UGInteractionAction
    ├── FSimpleDelegate OnCompleted   // 파이프라인이 바인딩
    ├── Execute(AActor* TargetActor)  // 순수 가상
    └── Finish()                      // 완료 시 호출 → OnCompleted 실행
```

### 2. 구체 액션 클래스 (신규)

| 클래스 | 역할 | 완료 조건 |
|---|---|---|
| `UGInteractionAction_MoveTo` | 캐릭터를 InteractPoint로 VInterpTo 이동 | 거리 임계값 이하 도달 |
| `UGInteractionAction_PlayMontage` | AnimMontage 재생 | Notify(FName) 발생 or MontageEnd fallback |
| `UGInteractionAction_Execute` | InternalInteract() 호출 | 즉시 완료 |

### 3. `AGInteractableActor` (수정)

```
기존:
- TArray<UGInteractionCondition*> Conditions  // 유지

추가:
- USceneComponent* InteractPoint              // 캐릭터 이동 목표 위치
- TArray<TSoftClassPtr<UGInteractionAction>> ActionClassPtrs  // 에디터에서 조합
- TArray<UGInteractionAction*> Actions        // 런타임 인스턴스
```

`Interact()` 변경:
- 기존: `InternalInteract()` 직접 호출
- 변경: `GInteractionComponent`에 액션 파이프라인 실행 요청

### 4. `UGInteractionActionPipeline` (신규 또는 GInteractionComponent 내부)

```
- TArray<UGInteractionAction*> Actions
- int32 CurrentIndex
- AActor* TargetActor

Run(Actions, TargetActor)  // 파이프라인 시작
RunNextAction()            // 현재 인덱스 액션 실행, OnCompleted 바인딩
```

### 5. `GInteractionComponent` (수정)

- 입력 잠금/해제 담당 (`SetIgnoreMoveInput`)
- 파이프라인 소유 및 실행
- `Tick`: `MoveTo` 액션 진행 중일 때 VInterpTo 처리

---

## 액션 완료 흐름 (델리게이트 체인)

```cpp
// 파이프라인
void RunNextAction()
{
    if (CurrentIndex >= Actions.Num()) { return; } // 완료

    UGInteractionAction* Action = Actions[CurrentIndex++];
    Action->OnCompleted.BindUObject(this, &ThisClass::RunNextAction);
    Action->Execute(TargetActor);
}

// 각 액션
void Finish()
{
    OnCompleted.ExecuteIfBound(); // → RunNextAction() 재귀 호출
}
```

---

## 시나리오별 Actions 배열 구성 (에디터)

| 인터랙션 종류 | Actions 배열 |
|---|---|
| 열쇠 줍기 (단순) | `[Execute]` |
| 상자 열기 | `[MoveTo, PlayMontage, Execute]` |
| 연출 있는 이벤트 | `[MoveTo, PlayMontage, Execute, PlayMontage]` |

---

## AnimNotify 결합도 처리

- `UGInteractionAction_PlayMontage`가 `FName NotifyName` 프로퍼티 보유
- `OnPlayMontageNotifyBegin` 델리게이트로 해당 이름 감지 → `Finish()`
- `NotifyName`이 비어있으면 `OnMontageEnded`로 fallback
- Notify 자체는 범용 AnimNotify (코드 종속 없음)

---

## 입력 잠금

- 파이프라인 `Run()` 시작 시: `PlayerController->SetIgnoreMoveInput(true)`
- 모든 액션 완료 시: `SetIgnoreMoveInput(false)`

---

## 추가/변경 파일 목록

| 파일 | 상태 |
|---|---|
| `Data/Interact/GInteractionAction.h/.cpp` | 신규 (베이스) |
| `Data/Interact/GInteractionAction_MoveTo.h/.cpp` | 신규 |
| `Data/Interact/GInteractionAction_PlayMontage.h/.cpp` | 신규 |
| `Data/Interact/GInteractionAction_Execute.h/.cpp` | 신규 |
| `Component/GInteractionActionPipeline.h/.cpp` | 신규 |
| `Gimmick/GInteractableActor.h/.cpp` | 수정 |
| `Component/GInteractionComponent.h/.cpp` | 수정 |
| `Interface/GInteractable.h` | 수정 (필요시) |
