#include "NodeInspectorUI.h"

#include "imgui.h"
#include "Node3D.h"
#include "NodeMeshInstance3D.h"
#include "Core/ClassDB.h"

class Node3D;

void NodeInspectorUI::DrawGUI()
{
    Node::DrawGUI();
    
    ImGui::Begin("Inspector");

    if (ViewedNode)
    {
        ViewedNode->DrawInspectorWidget();
        
        const auto visitor = overload
        {
            [](PropertyView& p, Transform& v){ ImGui::DrawImGuiTransformControl(p, &v); },
            [](PropertyView& p, std::string& v){ ImGui::DrawImGuiStringControl(p, &v); },
            [](PropertyView& p, bool& v){ ImGui::DrawImGuiCheckboxControl(p, &v); },
            [](PropertyView& p, Vector4& v){ ImGui::DrawImGuiVector4Control(p, &v); },
            [](PropertyView& p, Vector3& v){ ImGui::DrawImGuiVector3Control(p, &v); },
            [](PropertyView& p, Vector2& v){ ImGui::DrawImGuiVector2Control(p, &v); },
            [](PropertyView& p, float& v){ ImGui::DrawImGuiFloatControl(p, &v); },
            [](PropertyView& p, int& v){ ImGui::DrawImGuiIntControl(p, &v); },
            [](PropertyView& p, auto v) { std::cout << "Invalid variant for drawing: " << p.name() << "\n"; }
        };


        for (auto p : ViewedNode->GetPropertiesFromThis())
        {
            p.visit(visitor, ViewedNode.Get());
        }
    }
    
    ImGui::End();
}
