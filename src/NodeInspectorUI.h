#pragma once
#include "PropertyDrawUtility.h"
#include "Nodes/Node.h"
#include "Utility/WeakRef.h"

class NodeInspectorUI : public Node
{
public:
    NodeInspectorUI() = default;

    void DrawGUI() override;

    void SetViewedNode(Node* node)
    {
        ViewedNode = node;
    }
    WeakRef<Node> ViewedNode;

    PropertyDrawUtility visitor;
};

