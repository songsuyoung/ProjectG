#include "GInteractionAction.h"

void UGInteractionAction::Finish()
{
    OnCompleted.ExecuteIfBound();
}
