#pragma once
#include <rocket/rocket.hpp>

#include "imgui.h"
#include "Node.h"
#include "NodeEditorUI.h"
#include "Utility/WeakRef.h"

class NodeEditorUI;

class NodeSceneTreeUI : public Node
{
public:
    NodeSceneTreeUI() = default;

    explicit NodeSceneTreeUI(const std::string& name)
        : Node(name)
    {
    }

    void DrawGUI() override;

    rocket::signal<void(Node*)> OnNodeSelected;

    void SelectNode(Node* node);
    WeakRef<Node> SelectedNode;
private:
    void DrawNodeTree(Node *node, int& idx_count) {

        ImGuiTreeNodeFlags showArrow = node->GetChildren<Node>().empty() ? ImGuiTreeNodeFlags_Leaf : 0;
        ImGuiTreeNodeFlags selected = node == SelectedNode ? ImGuiTreeNodeFlags_Selected : 0;
        ImGuiTreeNodeFlags flags = showArrow | selected | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
        bool open = ImGui::TreeNodeEx((node->GetName() + "##" + std::to_string(idx_count-1)).c_str(), flags);
        idx_count++;
        bool bIsEditorUI = node->GetAncestorOfType<NodeEditorUI>() != nullptr || node->IsA<NodeEditorUI>();
        
        bool bCanDelete = !bIsEditorUI && node != node->GetRootNode();
        bool bCanDrag = !bIsEditorUI && node != node->GetRootNode();
        bool bCanDrop = !bIsEditorUI;
        
        if (ImGui::IsItemClicked())
        {
            SelectNode(node);
        }
        if (ImGui::IsItemClicked(1))
        {
            SelectNode(node);
            ImGui::OpenPopup("SceneNodeContextMenu");
        }

        if (bCanDrag && ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("SCENE_NODE", &node, sizeof(Node*));
            ImGui::EndDragDropSource();
        }
        if (bCanDrop && ImGui::BeginDragDropTarget())
        {
            const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_NODE");
            if (payload)
            {
                Node* draggedNode = *(Node**)payload->Data;
                if (draggedNode != node && !node->IsDescendantOf(draggedNode))
                {
                    if (draggedNode->GetParent() == node && node->GetParent() != nullptr)
                    {
                        auto n = draggedNode->RemoveFromParent();
                        node->GetParent()->AddChild(std::move(n), true);
                    }
                    else
                    {
                        auto n = draggedNode->RemoveFromParent();
                        node->AddChild(std::move(n), true);
                    }

                }
            }
            ImGui::EndDragDropTarget();
        }
        
        // context menu
        if (SelectedNode == node)
        {
            if (ImGui::BeginPopup("SceneNodeContextMenu"))
            {
                ImGui::Text("Options");
                ImGui::Separator();
                
                if (bCanDelete && ImGui::MenuItem("Delete"))
                {
                    node->RemoveFromParent();
                    SelectNode(nullptr);
                }
                ImGui::EndPopup();
            }
        }


        // recursive call for children
        if (open)
        {
            for (auto child : node->GetChildren())
            {
                DrawNodeTree(child, idx_count);
            }
            ImGui::TreePop();
        }
    }
};
