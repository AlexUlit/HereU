#include "BlueprintAssistActionsBase.h"

#include "BlueprintAssistGraphHandler.h"
#include "BlueprintAssistTabHandler.h"

TSharedPtr<FBAGraphHandler> FBAActionsBase::GetGraphHandler()
{
	return FBATabHandler::Get().GetActiveGraphHandler();
}

bool FBAActionsBase::CanExecuteActions() const
{
	return
		!GEditor->bIsSimulatingInEditor &&
		!GEditor->PlayWorld &&
		FSlateApplication::Get().IsInitialized();
}
