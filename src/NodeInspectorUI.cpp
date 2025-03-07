#include "NodeInspectorUI.h"

#include "imgui.h"
#include "PropertyDrawUtility.h"
#include "SceneSerializer.h"
#include "Nodes/Node3D.h"
#include "Nodes/NodeMeshInstance3D.h"
#include "Core/ClassDB.h"

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
            p.visit(visitor, ViewedNode.Get());
        }

        if (ImGui::Button("SerializeTest"))
        {
            SceneSerializer s;
            s.SerializeScene(ViewedNode.Get(), ENGINE_RESOURCE_DIR"/FunnyScene.json");
        }
    }


    
    ImGui::End();
}
