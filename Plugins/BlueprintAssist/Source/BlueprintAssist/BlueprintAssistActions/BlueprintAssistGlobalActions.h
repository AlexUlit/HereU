#pragma once

#include "CoreMinimal.h"
#include "BlueprintAssistActionsBase.h"

class BLUEPRINTASSIST_API FBAGlobalActionsBase : public FBAActionsBase
{
public:
	bool CanOpenEditDetailsMenu() const;
	bool HasWorkflowModes() const;
};

class BLUEPRINTASSIST_API FBAGlobalActions final : public FBAGlobalActionsBase
{
public:
	TSharedPtr<FUICommandList> GlobalCommands;

	virtual void Init() override;

	void DebugPrintGraphUnderCursor() const;
	void OpenFocusSearchBoxMenu();
	void OpenEditDetailsMenu();
	void OpenWindowMenu();
	void OpenBlueprintAssistHotkeyMenu();
	void OpenTabSwitcherMenu();
	void ToggleFullscreen();
	void SwitchWorkflowMode();
	void OpenAssetCreationMenu();
};
