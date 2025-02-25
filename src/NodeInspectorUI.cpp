#include "NodeInspectorUI.h"

#include "imgui.h"

void NodeInspectorUI::DrawGUI()
{
    Node::DrawGUI();
    
    ImGui::Begin("Inspector");

    if (ViewedNode)
    {
        ViewedNode->DrawInspectorWidget();
    }
    
    ImGui::End();
}
