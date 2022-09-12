// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintAssistTypes.h"

class FBAGraphHandler;

class BLUEPRINTASSIST_API SBlueprintAssistGraphOverlay : public SOverlay
{
	SLATE_BEGIN_ARGS(SBlueprintAssistGraphOverlay) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<FBAGraphHandler> InOwnerGraphHandler);

	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	void AddHighlightedPin(const FBAGraphPinHandle& PinHandle, const FLinearColor& Color);
	void AddHighlightedPin(UEdGraphPin* Pin, const FLinearColor& Color);

	void RemoveHighlightedPin(const FBAGraphPinHandle& PinHandle);
	void RemoveHighlightedPin(UEdGraphPin* Pin);

protected:
	TSharedPtr<FBAGraphHandler> OwnerGraphHandler;
	TMap<FBAGraphPinHandle, FLinearColor> PinsToHighlight;

	const FSlateBrush* CachedBorderBrush = nullptr;
};
