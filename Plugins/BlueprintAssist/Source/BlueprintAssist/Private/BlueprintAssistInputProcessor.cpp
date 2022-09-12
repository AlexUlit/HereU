// Copyright 2021 fpwong. All Rights Reserved.

#include "BlueprintAssistInputProcessor.h"

#include "BlueprintAssistGlobals.h"
#include "BlueprintAssistGraphHandler.h"
#include "BlueprintAssistModule.h"
#include "BlueprintAssistTabHandler.h"
#include "BlueprintAssistToolbar.h"
#include "SGraphPanel.h"
#include "BlueprintAssist/BlueprintAssistObjects/BARootObject.h"

static TSharedPtr<FBAInputProcessor> BAInputProcessorInstance;

void FBAInputProcessor::Create()
{
	BAInputProcessorInstance = MakeShareable(new FBAInputProcessor());
	FSlateApplication::Get().RegisterInputPreProcessor(BAInputProcessorInstance);
}

FBAInputProcessor& FBAInputProcessor::Get()
{
	return *BAInputProcessorInstance;
}

FBAInputProcessor::FBAInputProcessor()
{
	GlobalActions.Init();
	GraphActions.Init();
	NodeActions.Init();
	PinActions.Init();
	TabActions.Init();
	BlueprintActions.Init();

	CommandLists = {
		GlobalActions.GlobalCommands,
		TabActions.TabCommands,
		TabActions.ActionMenuCommands,
		GraphActions.GraphCommands,
		GraphActions.GraphReadOnlyCommands,
		NodeActions.SingleNodeCommands,
		NodeActions.MultipleNodeCommands,
		NodeActions.MultipleNodeCommandsIncludingComments,
		PinActions.PinCommands,
		PinActions.PinEditCommands,
		BlueprintActions.BlueprintCommands
	};
}

FBAInputProcessor::~FBAInputProcessor() {}

void FBAInputProcessor::Cleanup()
{
	if (FSlateApplication::IsInitialized())
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(BAInputProcessorInstance);
	}

	BAInputProcessorInstance.Reset();
}

void FBAInputProcessor::Tick(
	const float DeltaTime,
	FSlateApplication& SlateApp,
	TSharedRef<ICursor> Cursor)
{
	if (IsGameRunningOrCompiling())
	{
		return;
	}

	GraphHandler = FBATabHandler::Get().GetActiveGraphHandler();

	FBATabHandler::Get().Tick(DeltaTime);

	if (UBARootObject* RootObject = FBlueprintAssistModule::Get().GetRootObject())
	{
		RootObject->Tick();
	}
}

bool FBAInputProcessor::HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	if (OnKeyOrMouseDown(SlateApp, InKeyEvent.GetKey()))
	{
		return true;
	}

	// TODO: Perhaps implement a NavigationConfig, so users can't change focus on widgets
	// See FSlateApplication::SetNavigationConfig

	if (IsGameRunningOrCompiling())
	{
		return false;
	}

	if (SlateApp.IsInitialized())
	{
		GraphHandler = FBATabHandler::Get().GetActiveGraphHandler();

		// process toolbar commands
		if (FBAToolbar::Get().BlueprintAssistToolbarActions->ProcessCommandBindings(
			InKeyEvent.GetKey(),
			SlateApp.GetModifierKeys(),
			InKeyEvent.IsRepeat()))
		{
			return true;
		}

		if (GlobalActions.GlobalCommands->ProcessCommandBindings(
			InKeyEvent.GetKey(),
			FSlateApplication::Get().GetModifierKeys(),
			InKeyEvent.IsRepeat()))
		{
			return true;
		}

		if (BlueprintActions.HasOpenBlueprintEditor())
		{
			if (BlueprintActions.BlueprintCommands->ProcessCommandBindings(InKeyEvent.GetKey(), FSlateApplication::Get().GetModifierKeys(), InKeyEvent.IsRepeat()))
			{
				return true;
			}
		}

		if (!GraphHandler.IsValid())
		{
			//UE_LOG(LogBlueprintAssist, Warning, TEXT("Invalid graph handler"));
			return false;
		}

		// cancel graph handler ongoing processes
		if (InKeyEvent.GetKey() == EKeys::Escape)
		{
			GraphHandler->CancelSizeTimeoutNotification();
			GraphHandler->CancelCachingNotification();
			GraphHandler->CancelFormattingNodes();
			GraphHandler->ResetTransactions();
		}

		TSharedPtr<SDockTab> Tab = GraphHandler->GetTab();
		if (!Tab.IsValid() || !Tab->IsForeground())
		{
			//UE_LOG(LogBlueprintAssist, Warning, TEXT("Tab invalid or not foreground"));
			return false;
		}

		TSharedPtr<SWidget> KeyboardFocusedWidget = SlateApp.GetKeyboardFocusedWidget();
		// if (KeyboardFocusedWidget.IsValid())
		// {
		// 	UE_LOG(LogBlueprintAssist, Warning, TEXT("%s | %s"), *KeyboardFocusedWidget->GetTypeAsString(), *KeyboardFocusedWidget->ToString());
		// }
		// else
		// {
		// 	UE_LOG(LogBlueprintAssist, Warning, TEXT("No keyboard focused widget!"));
		// }

		// try process graph action menu hotkeys
		TSharedPtr<SWindow> Menu = SlateApp.GetActiveTopLevelWindow();
		if (Menu.IsValid())
		{
			//UE_LOG(LogBlueprintAssist, Warning, TEXT("Top Level window %s | %s"), *Menu->GetTitle().ToString(), *Menu->ToString());
			if (Menu->GetContent()->GetTypeAsString().Contains("SMenuContentWrapper"))
			{
				TSharedPtr<SWidget> ActionMenu = FBAUtils::GetChildWidget(Menu, "SGraphActionMenu");
				if (ActionMenu.IsValid())
				{
					//UE_LOG(LogBlueprintAssist, Warning, TEXT("Processing commands for action menu"));
					if (TabActions.ActionMenuCommands->ProcessCommandBindings(
						InKeyEvent.GetKey(),
						FSlateApplication::Get().GetModifierKeys(),
						InKeyEvent.IsRepeat()))
					{
						return true;
					}
				}
			}
		}

		// get the keyboard focused widget
		if (!Menu.IsValid() || !KeyboardFocusedWidget.IsValid())
		{
			//UE_LOG(LogBlueprintAssist, Warning, TEXT("Focus graph panel"));

			TSharedPtr<SGraphPanel> GraphPanel = GraphHandler->GetGraphPanel();
			SlateApp.SetKeyboardFocus(GraphPanel);
			KeyboardFocusedWidget = GraphPanel;
		}

		// process commands for when you are editing a user input widget
		if (FBAUtils::IsUserInputWidget(KeyboardFocusedWidget))
		{
			if (FBAUtils::GetParentWidgetOfType(KeyboardFocusedWidget, "SGraphPin").IsValid())
			{
				if (PinActions.PinEditCommands->ProcessCommandBindings(
					InKeyEvent.GetKey(),
					SlateApp.GetModifierKeys(),
					InKeyEvent.IsRepeat()))
				{
					return true;
				}
			}

			if (InKeyEvent.GetKey() == EKeys::Escape)
			{
				SlateApp.SetKeyboardFocus(GraphHandler->GetGraphPanel());
			}

			return false;
		}

		// process commands for when the tab is open
		if (TabActions.TabCommands->ProcessCommandBindings(
			InKeyEvent.GetKey(),
			SlateApp.GetModifierKeys(),
			InKeyEvent.IsRepeat()))
		{
			return true;
		}

		//UE_LOG(LogBlueprintAssist, Warning, TEXT("Process tab commands"));

		if (!GraphHandler->IsWindowActive())
		{
			//TSharedPtr<SWindow> CurrentWindow = FSlateApplication::Get().GetActiveTopLevelWindow();
			//const FString CurrentWindowStr = CurrentWindow.IsValid()
			//	? CurrentWindow->GetTitle().ToString()
			//	: "nullptr";

			//TSharedPtr<SWindow> GHWindow = GraphHandler->GetOrFindWindow();
			//FString GHWindowStr = GHWindow.IsValid() ? GHWindow->GetTitle().ToString() : "Nullptr";
			//UE_LOG(
			//	LogBlueprintAssist,
			//	Warning,
			//	TEXT("Graph Handler window is not active %s current window | GH Window %s"),
			//	*CurrentWindowStr,
			//	*GHWindowStr);
			return false;
		}

		// process commands for when the graph exists but is read only
		if (GraphActions.GraphReadOnlyCommands->ProcessCommandBindings(
			InKeyEvent.GetKey(),
			SlateApp.GetModifierKeys(),
			InKeyEvent.IsRepeat()))
		{
			return true;
		}

		// skip all other graph commands if read only
		if (GraphHandler->IsGraphReadOnly())
		{
			return false;
		}

		// process general graph commands
		if (GraphActions.GraphCommands->ProcessCommandBindings(
			InKeyEvent.GetKey(),
			SlateApp.GetModifierKeys(),
			InKeyEvent.IsRepeat()))
		{
			return true;
		}

		// process commands for which require a node to be selected
		if (GraphHandler->GetSelectedPin() != nullptr)
		{
			if (PinActions.PinCommands->ProcessCommandBindings(
				InKeyEvent.GetKey(),
				SlateApp.GetModifierKeys(),
				InKeyEvent.IsRepeat()))
			{
				return true;
			}
		}

		// process commands for which require a single node to be selected
		if (GraphHandler->GetSelectedNode() != nullptr)
		{
			//UE_LOG(LogBlueprintAssist, Warning, TEXT("Process node commands"));
			if (NodeActions.SingleNodeCommands->ProcessCommandBindings(
				InKeyEvent.GetKey(),
				SlateApp.GetModifierKeys(),
				InKeyEvent.IsRepeat()))
			{
				return true;
			}

			if (InKeyEvent.GetKey() == EKeys::Escape)
			{
				GraphHandler->GetGraphPanel()->SelectionManager.ClearSelectionSet();
			}
		}

		// process commands for which require multiple nodes to be selected
		if (GraphHandler->GetSelectedNodes().Num() > 0)
		{
			if (NodeActions.MultipleNodeCommands->ProcessCommandBindings(
				InKeyEvent.GetKey(),
				SlateApp.GetModifierKeys(),
				InKeyEvent.IsRepeat()))
			{
				return true;
			}

			if (InKeyEvent.GetKey() == EKeys::Escape)
			{
				GraphHandler->GetGraphPanel()->SelectionManager.ClearSelectionSet();
			}
		}

		// process commands for which require multiple nodes (incl comments) to be selected
		if (GraphHandler->GetSelectedNodes(true).Num() > 0)
		{
			if (NodeActions.MultipleNodeCommandsIncludingComments->ProcessCommandBindings(
				InKeyEvent.GetKey(),
				SlateApp.GetModifierKeys(),
				InKeyEvent.IsRepeat()))
			{
				return true;
			}

			if (InKeyEvent.GetKey() == EKeys::Escape)
			{
				GraphHandler->GetGraphPanel()->SelectionManager.ClearSelectionSet();
			}
		}
	}
	else
	{
		UE_LOG(LogBlueprintAssist, Error, TEXT("HandleKeyDown: Slate App not initialized"));
	}
	return false;
}

bool FBAInputProcessor::HandleKeyUpEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent)
{
	if (OnKeyOrMouseUp(SlateApp, InKeyEvent.GetKey()))
	{
		return true;
	}

	return false;
}

bool FBAInputProcessor::HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	if (OnKeyOrMouseDown(SlateApp, MouseEvent.GetEffectingButton()))
	{
		return true;
	}

	if (IsGameRunningOrCompiling())
	{
		return false;
	}

	TSharedPtr<FBAGraphHandler> MyGraphHandler = FBATabHandler::Get().GetActiveGraphHandler();

	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (MyGraphHandler.IsValid())
		{
			TSharedPtr<SGraphPanel> GraphPanel = MyGraphHandler->GetGraphPanel();
			if (MyGraphHandler->GetGraphPanel().IsValid())
			{
				if (GetDefault<UBASettings>()->bEnableShiftDraggingNodes)
				{
					TSharedPtr<SGraphNode> HoveredNode = FBAUtils::GetHoveredGraphNode(GraphPanel);
					if (HoveredNode)
					{
						bIsDragging = true;
						LastMousePos = FBAUtils::SnapToGrid(FBAUtils::ScreenSpaceToPanelCoord(MyGraphHandler->GetGraphPanel(), MouseEvent.GetScreenSpacePosition()) - NodeGrabOffset);
						NodeGrabOffset = HoveredNode->GetCachedGeometry().AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
					}
				}
				
				TSharedPtr<SGraphPin> GraphPin = FBAUtils::GetHoveredGraphPin(GraphPanel);
				if (GraphPin.IsValid())
				{
					UEdGraphPin* Pin = GraphPin->GetPinObj();

					MyGraphHandler->SetSelectedPin(Pin);

					GraphPanel->SelectionManager.SelectSingleNode(Pin->GetOwningNode());
				}
			}
		}

		if (GetMutableDefault<UBASettings>()->bCustomDebug == 100)
		{
			auto ActiveWindow = FSlateApplication::Get().GetActiveTopLevelWindow();
			TSharedPtr<SGraphPanel> GraphPanel = StaticCastSharedPtr<SGraphPanel>(FBAUtils::GetChildWidget(ActiveWindow, "SGraphPanel"));
			if (GraphPanel.IsValid())
			{
				TSharedPtr<SGraphNode> GraphNode = FBAUtils::GetHoveredGraphNode(GraphPanel);
				if (GraphNode.IsValid())
				{
					UEdGraphNode* Node = GraphNode->GetNodeObj();
					FBAUtils::PrintNodeInfo(Node);
				}
			}
		}
	}

	// Fix ongoing transactions being canceled via spawn node event on the graph. See FBlueprintEditor::OnSpawnGraphNodeByShortcut.
	if (MyGraphHandler.IsValid() && MyGraphHandler->HasActiveTransaction())
	{
		if (TSharedPtr<SGraphPanel> GraphPanel = MyGraphHandler->GetGraphPanel())
		{
			if (GraphPanel->IsHovered())
			{
				return true;
			}
		}
	}

	return false;
}

bool FBAInputProcessor::HandleMouseButtonUpEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	if (OnKeyOrMouseUp(SlateApp, MouseEvent.GetEffectingButton()))
	{
		return true;
	}

	if (MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		bIsDragging = false;
	}

	return false;
}

bool FBAInputProcessor::HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent)
{
	if (IsGameRunningOrCompiling())
	{
		return false;
	}

	bool bBlocking = false;
	TSharedPtr<FBAGraphHandler> MyGraphHandler = FBATabHandler::Get().GetActiveGraphHandler();

	if (MyGraphHandler.IsValid() && MyGraphHandler->GetGraphPanel().IsValid())
	{
		const FVector2D NewMousePos = FBAUtils::SnapToGrid(FBAUtils::ScreenSpaceToPanelCoord(MyGraphHandler->GetGraphPanel(), MouseEvent.GetScreenSpacePosition()) - NodeGrabOffset);
		const FVector2D Delta = NewMousePos - LastMousePos;

		const auto BASettings = GetDefault<UBASettings>();

		if (BASettings->bEnableShiftDraggingNodes && bIsDragging)
		{
			OnMouseDrag(SlateApp, NewMousePos, Delta);
		}

		// process extra drag nodes
		for (const FInputChord& Chord : BASettings->AdditionalDragNodesChords)
		{
			if (IsInputChordDown(Chord))
			{
				TSet<UEdGraphNode*> NodesToMove = MyGraphHandler->GetSelectedNodes();
				for (UEdGraphNode* Node : NodesToMove)
				{
					// if (!SelectedNodes.Contains(Node))
					{
						Node->Modify();
						Node->NodePosX += Delta.X;
						Node->NodePosY += Delta.Y;
					}
				}

				bBlocking = NodesToMove.Num() > 0;
			}
		}

		LastMousePos = NewMousePos;
	}

	return bBlocking;
}

void FBAInputProcessor::OnMouseDrag(FSlateApplication& SlateApp, const FVector2D& MousePos, const FVector2D& Delta)
{
	if (SlateApp.GetModifierKeys().IsShiftDown())
	{
		TSet<UEdGraphNode*> NodesToMove;

		// grab all linked nodes to move from the selected nodes
		TSet<UEdGraphNode*> SelectedNodes = GraphHandler->GetSelectedNodes();
		for (UEdGraphNode* SelectedNode : SelectedNodes)
		{
			NodesToMove.Append(FBAUtils::GetNodeTree(SelectedNode));
		}

		for (UEdGraphNode* Node : NodesToMove)
		{
			if (!SelectedNodes.Contains(Node))
			{
				Node->Modify();
				Node->NodePosX += Delta.X; 
				Node->NodePosY += Delta.Y;
			}
		}
	}
}

bool FBAInputProcessor::OnKeyOrMouseDown(FSlateApplication& SlateApp, const FKey& Key)
{
	KeysDown.Add(Key);
	KeysDownStartTime.Add(Key, FSlateApplication::Get().GetCurrentTime());

	if (IsGameRunningOrCompiling())
	{
		return false;
	}

	if (GraphHandler)
	{
		if (auto GraphPanel = GraphHandler->GetGraphPanel())
		{
			const UBASettings* BASettings = GetDefault<UBASettings>();
			for (const FInputChord& Chord : BASettings->AdditionalDragNodesChords)
			{
				if (IsInputChordDown(Chord))
				{
					if (TSharedPtr<SGraphNode> HoveredNode = FBAUtils::GetHoveredGraphNode(GraphPanel))
					{
						const TSet<UEdGraphNode*> SelectedNodes = GraphHandler->GetSelectedNodes(true);
						if (!SelectedNodes.Contains(HoveredNode->GetNodeObj()))
						{
							GraphHandler->SelectNode(HoveredNode->GetNodeObj());
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

bool FBAInputProcessor::OnKeyOrMouseUp(FSlateApplication& SlateApp, const FKey& Key)
{
	bool bBlocking = false;

	// process extra drag nodes
	const auto BASettings = GetDefault<UBASettings>();
	for (const FInputChord& Chord : BASettings->AdditionalDragNodesChords)
	{
		if (IsInputChordDown(Chord, Key) && GetKeyDownDuration(Key) > BASettings->ClickTime)
		{
			bBlocking = true;
			break;
		}
	}

	KeysDown.Remove(Key);
	KeysDownStartTime.Remove(Key);

	return bBlocking;
}

bool FBAInputProcessor::CanExecuteCommand(TSharedRef<const FUICommandInfo> Command) const
{
	for (TSharedPtr<FUICommandList> CommandList : CommandLists)
	{
		if (const FUIAction* Action = CommandList->GetActionForCommand(Command))
		{
			return Action->CanExecute();
		}
	}

	return false;
}

bool FBAInputProcessor::TryExecuteCommand(TSharedRef<const FUICommandInfo> Command)
{
	for (TSharedPtr<FUICommandList> CommandList : CommandLists)
	{
		if (const FUIAction* Action = CommandList->GetActionForCommand(Command))
		{
			if (Action->CanExecute())
			{
				return Action->Execute();
			}
		}
	}

	return false;
}

bool FBAInputProcessor::IsGameRunningOrCompiling() const
{
	return GEditor->bIsSimulatingInEditor || GEditor->PlayWorld != nullptr || FBAUtils::IsCompilingCode();
}

bool FBAInputProcessor::IsInputChordDown(const FInputChord& Chord)
{
	const FModifierKeysState ModKeysState = FSlateApplication::Get().GetModifierKeys();
	const bool AreModifiersDown = ModKeysState.AreModifersDown(EModifierKey::FromBools(Chord.bCtrl, Chord.bAlt, Chord.bShift, Chord.bCmd));
	return KeysDown.Contains(Chord.Key) && AreModifiersDown;
}

bool FBAInputProcessor::IsInputChordDown(const FInputChord& Chord, const FKey Key)
{
	const FModifierKeysState ModKeysState = FSlateApplication::Get().GetModifierKeys();
	const bool AreModifiersDown = ModKeysState.AreModifersDown(EModifierKey::FromBools(Chord.bCtrl, Chord.bAlt, Chord.bShift, Chord.bCmd));
	return Chord.Key == Key && AreModifiersDown;
}

double FBAInputProcessor::GetKeyDownDuration(const FKey Key)
{
	if (const double* FoundTime = KeysDownStartTime.Find(Key))
	{
		return FSlateApplication::Get().GetCurrentTime() - (*FoundTime);
	}

	return -1.0f;
}
