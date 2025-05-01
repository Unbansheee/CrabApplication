module;
#include "imgui.h"

module node_inspector_ui;
import Engine.PropertyDraw;
import Engine.SceneSerializer;

class Node3D;

void NodeInspectorUI::DrawGUI()
{
    Node::DrawGUI();
    
    ImGui::Begin("Inspector");

    if (ViewedNode)
    {
        ViewedNode->DrawInspectorWidget();

        std::string currentCategory;
        bool drawCurrentCategory = true;

        auto& props = ViewedNode->GetPropertiesFromThis();
        for (auto p : props)
        {

            if (p.flags & PropertyFlags::HideFromInspector) continue;
            if (currentCategory != p.ownerClass)
            {
                currentCategory = p.ownerClass;
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 7));
                ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.15, 0.15, 0.15, 1));
                ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(0.15, 0.15, 0.15, 1));
                drawCurrentCategory = ImGui::CollapsingHeader(currentCategory.c_str());
                ImGui::PopStyleColor(2);
                ImGui::PopStyleVar(1);
            }

            
            if (drawCurrentCategory){
                p.visit(visitor, ViewedNode.Get());
            }

        }
    }
    
    ImGui::End();
}
