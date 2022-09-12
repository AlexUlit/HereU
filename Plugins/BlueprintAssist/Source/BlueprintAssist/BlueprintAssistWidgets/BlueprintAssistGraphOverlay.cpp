#include "BlueprintAssistGraphOverlay.h"

#include "BlueprintAssistGraphHandler.h"
#include "BlueprintAssistUtils.h"
#include "SGraphPanel.h"

void SBlueprintAssistGraphOverlay::Construct(const FArguments& InArgs, TSharedPtr<FBAGraphHandler> InOwnerGraphHandler)
{
	OwnerGraphHandler = InOwnerGraphHandler;
	SetVisibility(EVisibility::HitTestInvisible);
	CachedBorderBrush = FCoreStyle::Get().GetBrush(TEXT("ExpandableArea.Border"));
}

int32 SBlueprintAssistGraphOverlay::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const int32 OutgoingLayer = SOverlay::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	// do nothing if the current graph handler is not our owner graph handler (or is null)
	TSharedPtr<FBAGraphHandler> CurrentGraphHandler = FBAUtils::GetCurrentGraphHandler();
	if (!CurrentGraphHandler || CurrentGraphHandler != OwnerGraphHandler)
	{
		return OutgoingLayer;
	}

	TSharedPtr<SGraphPanel> GraphPanel = CurrentGraphHandler->GetGraphPanel();
	if (!GraphPanel)
	{
		return OutgoingLayer;
	}

	// highlight pins
	for (auto Kvp : PinsToHighlight)
	{
		FBAGraphPinHandle PinHandle = Kvp.Key;
		FLinearColor Color = Kvp.Value;

		if (UEdGraphPin* Pin = PinHandle.GetPin())
		{
			if (TSharedPtr<SGraphPin> GraphPin = FBAUtils::GetGraphPin(GraphPanel, Pin))
			{
				const FSlateRect PinBounds = FBAUtils::GetPinBounds(GraphPin);
				if (GraphPanel->IsRectVisible(PinBounds.GetBottomRight(), PinBounds.GetTopLeft()))
				{
					const FPaintGeometry PaintGeometry = GraphPin->GetPaintSpaceGeometry().ToPaintGeometry();

					// Draw a border around the pin
					FSlateDrawElement::MakeBox(
						OutDrawElements,
						OutgoingLayer,
						PaintGeometry,
						CachedBorderBrush,
						ESlateDrawEffect::None,
						Color
					);
				}
			}
		}
	}

	return OutgoingLayer;
}

void SBlueprintAssistGraphOverlay::AddHighlightedPin(const FBAGraphPinHandle& PinHandle, const FLinearColor& Color)
{
	if (PinHandle.IsValid())
	{
		PinsToHighlight.Add(PinHandle, Color);
	}
}

void SBlueprintAssistGraphOverlay::AddHighlightedPin(UEdGraphPin* Pin, const FLinearColor& Color)
{
	AddHighlightedPin(FBAGraphPinHandle(Pin), Color);
}

void SBlueprintAssistGraphOverlay::RemoveHighlightedPin(const FBAGraphPinHandle& PinHandle)
{
	PinsToHighlight.Remove(PinHandle);
}

void SBlueprintAssistGraphOverlay::RemoveHighlightedPin(UEdGraphPin* Pin)
{
	RemoveHighlightedPin(FBAGraphPinHandle(Pin));
}
