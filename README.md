# ProjectG

Unreal Engine 5.6 기반의 3인칭 RPG 프로젝트입니다.

## 개요

**엔진**: Unreal Engine 5.6  
**플랫폼**: Windows 64-bit

| 단계 | 기간 | 목표 | 기능 정리 |
|------|------|------|-----------|
| 1차 | 2026.07.25 ~ 2026.08.26 | 인터랙션 / UI / 인벤토리 / 대화 / 퀘스트 시스템 | https://six-river-e29.notion.site/1-3bf3350dbc4680fc9a60c88602b915bc?pvs=74 |
| 2차 | 2026.08.27 ~ | SaveData, 월드 이동, 애니메이션, 카메라 Lock-on | |
## 주요 기능

### 인터랙션 시스템

기존에는 F키를 누르면 가장 가까운 오브젝트를 즉시 줍는 구조였다. 플레이어가 어떤 대상을 선택했는지 알 수 없고, UI나 이펙트를 끼워넣을 수 없는 문제가 있었다.

이를 **감지와 선택의 분리**로 해결했다.
- Sphere 오버랩으로 후보 목록을 이벤트 기반으로 유지
- 매 틱 스코어링(카메라 내적 + 거리)으로 포커스 대상 선정
- F키는 포커스된 대상에 대한 **확정 입력**으로만 동작
- Hold / Tap 방식 지원, 홀드 중 포커스가 바뀌어도 시작 시점의 대상 유지

**액션 파이프라인**: 상자 열기만 해도 캐릭터 이동 → 몽타주 재생 → 오픈 애니메이션이 순차로 필요하다. 각 동작을 UObject 단위(`MoveTo`, `PlayMontage`, `OpenUI`, `Talk` 등)로 분리하고 순차 실행하여, 문 열기와 상자 열기가 같은 액션 조합으로 재사용 가능하다.

**조건 시스템**: 인터랙션 가능 여부를 데이터 테이블 기반 Condition 오브젝트(아이템 보유, 레벨 체크 등)로 판단한다. 이 조건 시스템은 대화, 퀘스트에서도 동일하게 사용된다.

### 대화 시스템

처음에는 NPC에 `DialogueComponent`를 붙여 1:1 대화만 처리했으나, 트리거 박스나 레벨 시퀀스처럼 주체가 없는 대화를 커버할 수 없었다. `DialogueManager`로 승격시켜 모든 대화 요청을 한 곳에서 관리하도록 변경했다.

- 대화 중 인게임 입력이 차단되므로, 입력은 **UI에서** 받는다
- CommonUI InputAction 바인딩으로 Back/Apply 처리
- 이벤트 버스(GameplayTag)로 시스템에 전달

### 퀘스트 시스템

**상태 흐름**: `Available` → `Active` → `ReadyToComplete` → `Completed`

퀘스트 상태에 따라 NPC가 다른 대사를 출력한다.
- **Available**: 수락 전 대사 + 선택지(O/X)
- **Active**: 진행 중 대사 (없으면 기본 대사)
- **ReadyToComplete**: 완료 대사 + 보상 지급

전체 퀘스트 목록을 들고 있을 필요 없이, 현재 퀘스트의 `NextQuestID`만 따라가면 다음 퀘스트가 자동으로 활성화된다.

### 인벤토리 시스템

- 아이템 타입(`Material`, `Consumable`, `Equipment`)으로 분류
- UI에서 탭별로 필터링하여 표시
- UI는 컴포넌트에 사용을 **요청**만 하고, 실제 로직은 컴포넌트가 처리

### 제작 시스템

- 데이터 테이블로 레시피 정의 (재료 → 결과물)
- 인벤토리 연동으로 재료 확인 및 제작

### UI 시스템

- **CommonUI** + **GameplayTag** 기반 이벤트 통신
- 위젯 생성/닫기를 UClass가 아닌 GameplayTag로 처리

## 핵심 클래스

| 클래스 | 설명 |
|--------|------|
| `AGCharacter` | 플레이어 캐릭터. Interaction, Inventory, Quest 컴포넌트 보유 |
| `AGNPCCharacter` | NPC 캐릭터 |
| `UGInteractionComponent` | 후보 감지 + 스코어링 기반 포커스 선택 |
| `UGInteractionActionComponent` | 액션 파이프라인 순차 실행 |
| `UGInventoryComponent` | 아이템 저장, 획득, 사용 |
| `UGQuestComponent` | 퀘스트 상태 관리 및 목표 추적 |
| `UGDialogueManager` | 대화 트리 순회 및 상태 관리 |
| `UGEventManager` | GameplayTag 기반 이벤트 브로드캐스트 |
| `UGDataManager` | 데이터 테이블 접근 및 캐싱 |
| `UGConditionManager` | 조건 평가 (아이템, 레벨, 퀘스트 상태 등) |
| `AGInteractableActor` | 인터랙션 가능 액터 베이스 (조건, 우선순위, 홀드) |

## 프로젝트 구조

```
Source/
├── ProjectG/                          # 코어 모듈
│   ├── Animation/                     # AnimInstance
│   ├── Character/                     # Base, Player, NPC 캐릭터
│   ├── Component/                     # Hero, Interaction, Inventory, Quest
│   ├── Data/                          # 데이터 테이블 Row, Enum, GameplayTag
│   │   ├── Condition/                 # 조건 검사 (아이템, 레벨)
│   │   └── Interact/Action/           # 인터랙션 액션 (MoveTo, Talk 등)
│   ├── Gimmick/                       # Pickup, Openable 액터
│   ├── Interface/                     # IInteractable, IMessageReceiver
│   └── System/                        # GameInstance, Manager 클래스
│
└── ProjectGClient/                    # UI 모듈
    ├── System/                        # HUD, UIManager
    └── UI/                            # Inventory, Dialogue, Crafting 위젯
```

## 설계 원칙

- **Data-Driven**: 인터랙션, 대화, 퀘스트, 아이템, 조건 모두 데이터 테이블로 관리
- **이벤트 버스**: GameplayTag 기반 EventManager로 시스템 간 느슨한 결합
- **조건 시스템 공유**: 인터랙션 / 대화 / 퀘스트가 동일한 Condition 구조 사용
- **액션 파이프라인**: 인터랙션 동작을 UObject 단위로 분리, 재사용성 확보

## 의존성

- EnhancedInput
- GameplayTags
- CommonUI
- CommonInput
