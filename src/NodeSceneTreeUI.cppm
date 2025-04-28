module;

#pragma once
#include <rocket/rocket.hpp>
#include "ReflectionMacros.h"
#include "imgui.h"

export module node_scene_tree_ui;
import node_editor_scene_root;
import Engine.Object.Ref;
import Engine.Node;

export class NodeEditorUI;

export class NodeSceneTreeUI : public Node
{
public:
    CRAB_CLASS(NodeSceneTreeUI, Node)
    NodeSceneTreeUI() = default;

    void DrawGUI() override;

    Node* GetSelectedNode();
    void SetSelectedNode(Node* node);
    ObjectRef<Node> SceneRootOverride;
    bool bShowTypes = true;
private:
    void DrawNodeTree(ObjectRef<Node>& node, int& idx_count);
};
