#include "NodeSceneTreeUI.h"

#include "Application.h"
#include "imgui.h"

void NodeSceneTreeUI::DrawGUI()
{
    Node::DrawGUI();

    ImGui::Begin("Scene");

    int idx = 0;
    DrawNodeTree(Application::Get().GetSceneTree().GetRoot<Node>(), idx);

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
