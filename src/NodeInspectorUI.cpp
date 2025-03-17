module;
#include "imgui.h"

module node_inspector_ui;
import property_draw;
import scene_serializer;

class Node3D;

void NodeInspectorUI::DrawGUI()
{
    Node::DrawGUI();
    
    ImGui::Begin("Inspector");

    if (ViewedNode)
    {
        ViewedNode->DrawInspectorWidget();

        for (auto p : ViewedNode->GetPropertiesFromThis())
        {
            if ((uint32_t)p.flags & (uint32_t)Property::Flags::HideFromInspector) continue;
            p.visit(visitor, ViewedNode.Get());
        }
    }
    
    ImGui::End();
}
