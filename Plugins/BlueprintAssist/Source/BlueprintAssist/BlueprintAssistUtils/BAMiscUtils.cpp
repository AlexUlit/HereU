// Fill out your copyright notice in the Description page of Project Settings.

#include "BAMiscUtils.h"

#include "Editor/Kismet/Public/SSCSEditor.h"

#if BA_UE_VERSION_OR_LATER(5, 0)
#include "SSubobjectEditor.h"
#endif

AActor* FBAMiscUtils::GetSCSNodeDefaultActor(TSharedPtr<BA_SUBOBJECT_EDITOR_TREE_NODE> Node, UBlueprint* Blueprint)
{
#if BA_UE_VERSION_OR_LATER(5, 0)
	FSubobjectData* Data = Node->GetDataSource();
	return Data ? const_cast<AActor*>(Data->GetObjectForBlueprint<AActor>(Blueprint)) : nullptr;
#elif BA_UE_VERSION_OR_LATER(4, 26)
	return Node->GetEditableObjectForBlueprint<AActor>(Blueprint);
#else
	return (Blueprint != nullptr && Blueprint->GeneratedClass != nullptr) ? Blueprint->GeneratedClass->GetDefaultObject<AActor>() : nullptr;
#endif
}

bool FBAMiscUtils::IsSCSActorNode(TSharedPtr<BA_SUBOBJECT_EDITOR_TREE_NODE> Node)
{
#if BA_UE_VERSION_OR_LATER(5, 0)
	FSubobjectData* Source = Node->GetDataSource();
	return Source ? Source->IsActor() : false;
#elif BA_UE_VERSION_OR_LATER(4, 26)
	return Node->IsActorNode();
#else
	return Node->GetNodeType() == FSCSEditorTreeNode::ENodeType::RootActorNode;
#endif
}
