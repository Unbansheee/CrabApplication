#pragma once
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

    template<class... Ts>
    struct overload : Ts... {
        using Ts::operator()...;
    };

    template<class... Ts>
    overload(Ts...) -> overload<Ts...>;
};

