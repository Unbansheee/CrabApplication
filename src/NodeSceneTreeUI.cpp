#include "imgui.h"

module node_scene_tree_ui;
import application;
import scene_tree;

void NodeSceneTreeUI::DrawGUI()
{
    Node::DrawGUI();

    ImGui::Begin(Name.c_str());
    
    int idx = 0;
    if (SceneRootOverride)
    {
        DrawNodeTree(SceneRootOverride.Get(), idx);
    }
    else
    {
        DrawNodeTree(GetTree()->GetRoot<Node>(), idx);
    }
    
    ImGui::End();
}

void NodeSceneTreeUI::SelectNode(Node* node)
{
    if (node != SelectedNode)
    {
        SelectedNode = node;
        OnNodeSelected.invoke(node);
    }
}
