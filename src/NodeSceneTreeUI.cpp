#include "NodeSceneTreeUI.h"

#include "Application.h"
#include "imgui.h"
#include "Core/ClassDB.h"

void NodeSceneTreeUI::DrawGUI()
{
    Node::DrawGUI();

    ImGui::Begin("Scene");
    
    int idx = 0;
    DrawNodeTree(Application::Get().GetSceneTree().GetRoot<Node>(), idx);

    ImGui::End();

    ImGui::Begin("Add Nodes");

    auto classes = ClassDB::Get().GetClasses();
    for (auto& t : classes)
    {
        if (ImGui::Button(t.Name.c_str()))
        {
            Object* n = t.Initializer();
            Node* node = dynamic_cast<Node*>(n);
            auto instance = Node::InitializeNode(node, t.Name);
            if (SelectedNode)
            {
                SelectedNode->AddChild(std::move(instance));
            }
            else
            {
                Application::Get().GetSceneTree().GetRoot<Node>()->AddChild(std::move(instance));
            }
        }
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
