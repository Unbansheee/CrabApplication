#include "NodeSceneTreeUI.h"

#include "Application.h"
#include "imgui.h"
#include "Core/ClassDB.h"
#include "Resource/Resource.h"

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
