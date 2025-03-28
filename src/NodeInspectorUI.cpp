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

        for (auto p : ViewedNode->GetPropertiesFromThis())
        {
            if (p.flags & PropertyFlags::HideFromInspector) continue;
            p.visit(visitor, ViewedNode.Get());
        }
    }
    
    ImGui::End();
}
