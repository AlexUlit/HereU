// Copyright 2021 fpwong. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintAssist/BlueprintAssistActions/BlueprintAssistBlueprintActions.h"
#include "BlueprintAssist/BlueprintAssistActions/BlueprintAssistGlobalActions.h"
#include "BlueprintAssist/BlueprintAssistActions/BlueprintAssistGraphActions.h"
#include "BlueprintAssist/BlueprintAssistActions/BlueprintAssistNodeActions.h"
#include "BlueprintAssist/BlueprintAssistActions/BlueprintAssistPinActions.h"
#include "BlueprintAssist/BlueprintAssistActions/BlueprintAssistTabActions.h"

#include "EdGraph/EdGraphNode.h"
#include "Framework/Application/IInputProcessor.h"

class SDockTab;
class FBAGraphHandler;
class FBATabHandler;
class FUICommandList;

class BLUEPRINTASSIST_API FBAInputProcessor
	: public TSharedFromThis<FBAInputProcessor>
	, public IInputProcessor
{
public:
	virtual ~FBAInputProcessor() override;

	static void Create();

	static FBAInputProcessor& Get();

	void Cleanup();

	//~ Begin IInputProcessor Interface
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override;

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;
	virtual bool HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;

	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;
	virtual bool HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;

	virtual bool HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override;

	void OnMouseDrag(FSlateApplication& SlateApp, const FVector2D& MousePos, const FVector2D& Delta);

	bool OnKeyOrMouseDown(FSlateApplication& SlateApp, const FKey& Key);
	bool OnKeyOrMouseUp(FSlateApplication& SlateApp, const FKey& Key);

	bool bIsDragging = false;
	FVector2D LastMousePos;
	FVector2D NodeGrabOffset; 
	//~ End IInputProcessor Interface

	bool CanExecuteCommand(TSharedRef<const FUICommandInfo> Command) const;
	bool TryExecuteCommand(TSharedRef<const FUICommandInfo> Command);

	bool IsGameRunningOrCompiling() const;

	TSet<FKey> KeysDown;
	TMap<FKey, double> KeysDownStartTime; 

private:
	TSharedPtr<FBAGraphHandler> GraphHandler;

	FBAGlobalActions GlobalActions;
	FBATabActions TabActions;
	FBAGraphActions GraphActions;
	FBANodeActions NodeActions;
	FBAPinActions PinActions;
	FBABlueprintActions BlueprintActions;
	TArray<TSharedPtr<FUICommandList>> CommandLists;

	FBAInputProcessor();

#if ENGINE_MINOR_VERSION >= 26 || ENGINE_MAJOR_VERSION >= 5
	virtual const TCHAR* GetDebugName() const override { return TEXT("BlueprintAssistInputProcessor"); }
#endif

	bool IsInputChordDown(const FInputChord& Chord);

	bool IsInputChordDown(const FInputChord& Chord, const FKey Key);

	double GetKeyDownDuration(const FKey Key);
};
