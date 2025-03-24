module;
#pragma once
#include "ReflectionMacros.h"

export module node_inspector_ui;
import Engine.Node;
import Engine.Object.Ref;
import property_draw;

export class NodeInspectorUI : public Node
{
public:
    CRAB_CLASS(NodeInspectorUI, Node)

    NodeInspectorUI() = default;

    void DrawGUI() override;

    void SetViewedNode(Node* node)
    {
        ViewedNode = node;
    }
    ObjectRef<Node> ViewedNode;

    PropertyDrawUtility visitor;
};

