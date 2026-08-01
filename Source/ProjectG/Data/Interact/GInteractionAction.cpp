// Source/ProjectG/Data/Interact/GInteractionAction.cpp
#include "Data/Interact/GInteractionAction.h"

void UGInteractionAction::Finish()
{
    OnCompleted.ExecuteIfBound();
}
