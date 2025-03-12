#include "PropertyDrawUtility.h"

#include "imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"
#include "Nodes/Node.h"
#include "Resource/Resource.h"


void PropertyDrawUtility::operator()(PropertyView& prop, int& value)
{
    if (ImGui::DragInt(prop.name().c_str(), &value))
    {
        prop.set(value);
    }
}

void PropertyDrawUtility::operator()(PropertyView& prop, float& value)
{
    if (ImGui::DragFloat(prop.name().c_str(), &value))
    {
        prop.set(value);
    }
}

void PropertyDrawUtility::operator()(PropertyView& prop, bool& value)
{
    if (ImGui::Checkbox(prop.name().c_str(), &value))
    {
        prop.set(value);
    }
}

void PropertyDrawUtility::operator()(PropertyView& prop, std::string& value)
{
    if (ImGui::InputText(prop.name().c_str(), &value))
    {
        prop.set(value);
    }
}

void PropertyDrawUtility::operator()(PropertyView& prop, Vector2& value)
{
    if (ImGui::DragFloat2(prop.name().c_str(), &value.x))
    {
        prop.set(value);
    }
}

void PropertyDrawUtility::operator()(PropertyView& prop, Vector3& value)
{
    if (ImGui::DragFloat3(prop.name().c_str(), &value.x))
    {
        prop.set(value);
    }
}

void PropertyDrawUtility::operator()(PropertyView& prop, Vector4& value)
{
    if (ImGui::DragFloat4(prop.name().c_str(), &value.x))
    {
        prop.set(value);
    }
}

void PropertyDrawUtility::operator()(PropertyView& prop, Quat& value)
{
    
}

void PropertyDrawUtility::operator()(PropertyView& prop, Transform& value)
{
    bool dirty = false;
    ImGui::Text("Transform");
    
    if (ImGui::DragFloat3("Position", &value.Position.x, 0.1f))
    {
        dirty = true;
    }

    auto euler = glm::degrees(glm::eulerAngles(value.Orientation));
    if (ImGui::DragFloat3("Rotation", &euler.x, 0.1f))
    {
        value.Orientation = Quat(glm::radians(euler));
        dirty = true;
    }
    if (ImGui::DragFloat3("Scale", &value.Scale.x, 0.1f))
    {
        dirty = true;
    }
    
    if (dirty)
    {
        prop.set(value);
        if (auto node = dynamic_cast<Node*>(prop.object))
        {
            node->UpdateTransform();
        }
    }
}

void PropertyDrawUtility::operator()(PropertyView& prop, ResourceHandle& val)
{
    auto str = val.ResourceID.to_string();
    ImGui::Text(prop.name().c_str());
    ImGui::SameLine();
    ImGui::Text(val.ResourceID.to_string().c_str());
    if (ImGui::BeginDragDropTarget())
    {
        const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("RESOURCE_REF");
        if (payload)
        {
            SharedRef<Resource> resource = *(SharedRef<Resource>*)payload->Data;
            val = resource->Handle();
            prop.set(val);
        }

        ImGui::EndDragDropTarget();
    }
}
