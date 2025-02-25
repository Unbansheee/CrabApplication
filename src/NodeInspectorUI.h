#pragma once
#include "Node.h"
#include "Utility/WeakRef.h"

class NodeInspectorUI : public Node
{
public:
    NodeInspectorUI() = default;

    explicit NodeInspectorUI(const std::string& name)
        : Node(name)
    {
    }

    void DrawGUI() override;

    void SetViewedNode(Node* node)
    {
        ViewedNode = node;
    }
    WeakRef<Node> ViewedNode;
};
