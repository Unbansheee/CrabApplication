module;

#pragma once
#include <rocket/rocket.hpp>
#include "ReflectionMacros.h"
#include "imgui.h"

export module node_scene_tree_ui;
import node_editor_scene_root;
import object_ref;
import node;

export class NodeEditorUI;

export class NodeSceneTreeUI : public Node
{
public:
    CRAB_CLASS(NodeSceneTreeUI, Node)

    NodeSceneTreeUI() = default;

    void DrawGUI() override;

    rocket::signal<void(Node*)> OnNodeSelected;

    void SelectNode(Node* node);
    WeakRef<Node> SelectedNode;
    WeakRef<Node> SceneRootOverride;
private:
    void DrawNodeTree(Node *node, int& idx_count);
};
