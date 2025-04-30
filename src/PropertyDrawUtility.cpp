module;
#include "imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"


module Engine.PropertyDraw;
import node_content_browser_panel;
import Engine.Resource.ResourceManager;
import Engine.Node;
import Engine.Resource;
import Engine.Resource.Mesh;
import Engine.Reflection.Class;

void PropertyDrawUtility::operator()(PropertyView& prop, int& value)
{
    if (ImGui::DragInt(prop.displayName().c_str(), &value))
    {
        prop.set(value);
    }
}

void PropertyDrawUtility::operator()(PropertyView& prop, float& value)
{
    if (ImGui::DragFloat(prop.displayName().c_str(), &value))
    {
        prop.set(value);
    }
}

void PropertyDrawUtility::operator()(PropertyView& prop, bool& value)
{
    if (ImGui::Checkbox(prop.displayName().c_str(), &value))
    {
        prop.set(value);
    }
}

void PropertyDrawUtility::operator()(PropertyView& prop, std::string& value)
{
    if (ImGui::InputText(prop.displayName().c_str(), &value))
    {
        prop.set(value);
    }
}

void PropertyDrawUtility::operator()(PropertyView& prop, Vector2& value)
{
    if (ImGui::DragFloat2(prop.displayName().c_str(), &value.x))
    {
        prop.set(value);
    }
}

void PropertyDrawUtility::operator()(PropertyView& prop, Vector3& value)
{
    if (prop.flags() | PropertyFlags::ColorHint) {
        if (ImGui::ColorEdit3(prop.displayName().c_str(), &value.x, ImGuiColorEditFlags_Float)) {
            prop.set(value);
        }
    }
    else {
        if (ImGui::DragFloat3(prop.displayName().c_str(), &value.x))
        {
            prop.set(value);
        }
    }

}

void PropertyDrawUtility::operator()(PropertyView& prop, Vector4& value)
{
    if (prop.flags() | PropertyFlags::ColorHint) {
        if (ImGui::ColorEdit4(prop.displayName().c_str(), &value.x, ImGuiColorEditFlags_Float)) {
            prop.set(value);
        }
    }
    else {
        if (ImGui::DragFloat4(prop.displayName().c_str(), &value.x))
        {
            prop.set(value);
        }
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
        if (prop.objectClass && prop.objectClass->IsSubclassOf(Node::GetStaticClass())) {
            auto node = static_cast<Node*>(prop.object);
            node->UpdateTransform();
        }

    }
}


void PropertyDrawUtility::operator()(PropertyView& prop, std::shared_ptr<Resource>& val)
{
    auto res = val;
    std::string title = "Invalid Resource";
    if (res) title = res->GetName();

    ImGui::PushID(prop.displayName().c_str());
    ImGui::Text(prop.displayName().c_str());
    ImGui::SameLine();
    if (ImGui::Button("Load")) {
        ImGui::OpenPopup("ResourceSelector");
    }
    ImGui::SameLine();
    bool nodeOpen = ImGui::TreeNode("%s", title.c_str());
    if (ImGui::BeginDragDropTarget()) {
        auto payload = ImGui::AcceptDragDropPayload("RESOURCE_PATH");
        if (payload)
        {
            ResourcePathDragDropData* data = static_cast<ResourcePathDragDropData*>(payload->Data);
            auto res_ref = ResourceManager::Load(data->GetPath());

            prop.set(val);
        }
        
        ImGui::EndDragDropTarget();
    }

    if (nodeOpen)
    {
        if (res)
        {
            for (auto p : res->GetPropertiesFromThis())
            {
                if ((uint32_t)p.flags & (uint32_t)PropertyFlags::HideFromInspector) continue;
                p.visit(*this, res.get());
            }
        }
        ImGui::TreePop();
    }


    if (ImGui::BeginPopup("ResourceSelector"))
    {
        int i = 0;
        auto allresources = ResourceManager::GetAllResources();
        for (auto& r : allresources)
        {
            if (!r->IsA(*prop.property.reflectedObjectType)) continue;
            if (ImGui::Button((r->GetName() + "##" + std::to_string(i)).c_str()))
            {
                val = r;
                prop.set(val);
                //prop.set<StrongResourceRef>(val);
            }

            i++;

        }
        ImGui::EndPopup();
    }
    ImGui::PopID();
    
}

void PropertyDrawUtility::operator()(PropertyView &prop, ObjectRef<Object> &val) {
    ImGui::Text(prop.displayName().c_str());
    std::string name = val.IsValid() ? val->GetID().to_string() : "NULL";
    ImGui::Text(name.c_str());
}
