module;
#include "imgui.h"

module node_scene_tree_ui;
import application;
import scene_tree;

void NodeSceneTreeUI::DrawGUI()
{
    Node::DrawGUI();

    ImGui::Begin(Name.c_str());

    if (ImGui::BeginPopup("Add Node Menu"))
    {
        auto classes = ClassDB::Get().GetSubclassesOf<Node>();
        for (auto& t : classes)
        {
            if (!(t->HasFlag((uint32_t)ClassType::ClassFlags::EditorVisible))) continue;
            if (ImGui::Button(t->Name.string()))
            {
                Object* n = t->Initializer();
                Node* node = dynamic_cast<Node*>(n);
                auto instance = Node::InitializeNode(node, t->Name.string());
                if (SelectedNode)
                {
                    SelectedNode->AddChild(std::move(instance));
                }
                else if (SceneRootOverride)
                {
                    SceneRootOverride->AddChild(std::move(instance));
                }
                else
                {
                    GetTree()->GetRoot<Node>()->AddChild(std::move(instance));
                }
            }
        }
        ImGui::EndPopup();
    }

    if (ImGui::Button("Add Node"))
    {
        ImGui::OpenPopup("Add Node Menu");
    }
    
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

void NodeSceneTreeUI::DrawNodeTree(Node* node, int& idx_count)
{

    ImGuiTreeNodeFlags showArrow = node->GetChildren().empty() ? ImGuiTreeNodeFlags_Leaf : 0;
    ImGuiTreeNodeFlags selected = node == SelectedNode ? ImGuiTreeNodeFlags_Selected : 0;
    ImGuiTreeNodeFlags flags = showArrow | selected | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen;
    bool open = ImGui::TreeNodeEx((node->GetName() + "##" + std::to_string(idx_count-1)).c_str(), flags);
    idx_count++;
    //bool bIsEditorUI = node->GetAncestorOfType<NodeEditorUI>() != nullptr || node->IsA<NodeEditorUI>();
    bool bIsEditorUI = false;
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
                    draggedNode->Reparent(node->GetParent());
                }
                else
                {
                    draggedNode->Reparent(node);
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

            if (ImGui::MenuItem("Duplicate")) {
                auto n = node->Duplicate();
                node->GetParent()->AddChild(std::move(n));
            }

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
        node->ForEachChild([this, &idx_count](Node* child)
        {
            DrawNodeTree(child, idx_count);
        });
        if (auto treenode = Cast<NodeEditorSceneRoot>(node))
        {
            DrawNodeTree(treenode->GetSubtree().GetRoot<Node>(), idx_count);
        }
        ImGui::TreePop();
    }
}
