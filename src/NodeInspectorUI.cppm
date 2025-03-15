#pragma once

export module node_inspector_ui;
import node;
import object_ref;
import property_draw;

export class NodeInspectorUI : public Node
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

