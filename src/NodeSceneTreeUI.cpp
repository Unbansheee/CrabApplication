module;
#include "imgui.h"

module node_scene_tree_ui;
import Engine.Application;
import Engine.SceneTree;
import node_editor_ui;

void NodeSceneTreeUI::DrawGUI()
{
    Node::DrawGUI();

    ImGui::Begin(Name.c_str());
    if (ImGui::BeginPopup("Add Node Menu"))
    {
        auto classes = ClassDB::Get().GetSubclassesOf<Node>();
        for (auto& t : classes)
        {
            if (!(t->HasFlag(ClassFlags::EditorVisible))) continue;
            if (ImGui::Button(t->Name.string()))
            {
                Object* n = t->Initializer();
                Node* node = dynamic_cast<Node*>(n);
                auto instance = Node::InitializeNode(node, t->Name.string());
                if (auto editor = GetAncestorOfType<NodeEditorUI>())
                {
                    if (editor->SelectedNode)
                    {
                        auto child = editor->SelectedNode->AddChild(std::move(instance));
                        editor->SelectNode(child);
                    }
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
    if (SceneRootOverride.IsValid())
    {
        DrawNodeTree(SceneRootOverride, idx);
    }
    else
    {
        auto r = ObjectRef<Node>(GetTree()->GetRoot<Node>());
        DrawNodeTree(r, idx);
    }
    
    ImGui::End();
}

Node* NodeSceneTreeUI::GetSelectedNode()
{
    if (auto editor = GetAncestorOfType<NodeEditorUI>())
    {
        return editor->SelectedNode.Get();
    }
    return nullptr;
}

void NodeSceneTreeUI::SetSelectedNode(Node* node)
{
    if (auto editor = GetAncestorOfType<NodeEditorUI>())
    {
        return editor->SelectNode(node);
    }
}


void NodeSceneTreeUI::DrawNodeTree(ObjectRef<Node>& node, int& idx_count)
{
    if (!node.IsValid()) return;
    if (!node->GetTree()) return;
    ImGui::PushID(node->GetID().to_string().c_str());
    ImGuiTreeNodeFlags showArrow = node->GetChildren().empty() ? ImGuiTreeNodeFlags_Leaf : 0;
    ImGuiTreeNodeFlags selected = node == GetSelectedNode() ? ImGuiTreeNodeFlags_Selected : 0;
    ImGuiTreeNodeFlags flags = showArrow | selected | ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_FramePadding;

    ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.2, 0.2, 0.2, 1));
    ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.2, 0.2, 0.2, 1));

    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 5));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    bool open = ImGui::TreeNodeEx((node->GetName() + "##" + std::to_string(idx_count-1)).c_str(), flags);
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(2);
    idx_count++;
    //bool bIsEditorUI = node->GetAncestorOfType<NodeEditorUI>() != nullptr || node->IsA<NodeEditorUI>();
    bool bIsEditorUI = false;
    bool bCanDelete = !bIsEditorUI && node != node->GetRootNode();
    bool bCanDrag = !bIsEditorUI && node != node->GetRootNode();
    bool bCanDrop = !bIsEditorUI;
        
    if (ImGui::IsItemClicked())
    {
        SetSelectedNode(node.Get());
    }
    if (ImGui::IsItemClicked(1))
    {
        SetSelectedNode(node.Get());
        ImGui::OpenPopup("SceneNodeContextMenu");
    }

    if (bCanDrag && ImGui::BeginDragDropSource())
    {
        Node* n = node.Get();
        auto id = node->GetID();
        ImGui::SetDragDropPayload("SCENE_NODE", &id, sizeof(UID));
        ImGui::EndDragDropSource();
    }
    if (bCanDrop && ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("SCENE_NODE");
        if (payload && node.IsValid())
        {
            UID nodeID = *(UID*)payload->Data;
            //Node* draggedNode = *(Node**)payload->Data;
            Node* draggedNode = node->GetTree()->GetNodeByID(nodeID);
            if (draggedNode && draggedNode != node && !node->IsDescendantOf(draggedNode))
            {
                if (draggedNode->GetParent() == node && node->GetParent() != nullptr)
                {
                    draggedNode->Reparent(node->GetParent());
                }
                else
                {
                    draggedNode->Reparent(node.Get());
                }

            }
        }
        ImGui::EndDragDropTarget();
    }
        
    // context menu
    if (GetSelectedNode() == node)
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
                SetSelectedNode(nullptr);
            }
            ImGui::EndPopup();
        }
    }



    // recursive call for children
    if (open)
    {
        if (node.IsValid()) {
            node->ForEachChildSafe<Node>([this, &idx_count](ObjectRef<Node>& child)
            {
                if (child.IsValid()) {
                    DrawNodeTree(child, idx_count);
                }
            });

            if (node.IsValid()) {
                auto a = Cast<NodeEditorSceneRoot>(node.Get());
                if (a) {
                    if (Node* rootPtr = a->GetSubtree().GetRoot<Node>()) {
                        auto root = ObjectRef<Node>(rootPtr);
                        DrawNodeTree(root, idx_count);
                    }
                }
            }

        }

        ImGui::TreePop();
    }
    ImGui::PopID();
}
