module;
#include "imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"


module property_draw;
import node_content_browser_panel;
import resource_manager;
import node;
import resource;
import mesh_resource;

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

void PropertyDrawUtility::operator()(PropertyView& prop, ResourceRef& val)
{
    ImGui::Text(prop.name().c_str());
    ImGui::SameLine();

    /*
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
    */

    if (auto res = val.Get<Resource>()) {
        ImGui::Text("%s", res->GetResourcePath().c_str());
        if (ImGui::BeginDragDropTarget()) {
            auto payload = ImGui::AcceptDragDropPayload("RESOURCE_PATH");
            if (payload)
            {
                ResourcePathDragDropData* data = (ResourcePathDragDropData*)payload->Data;
                auto res_ref = ResourceManager::Load(data->GetPath());
                val = res_ref;
                prop.set(val);
            }
            
            ImGui::EndDragDropTarget();
            // Handle drag-drop from resource browser
        }
    } else {
        ImGui::Text("None");
        if (ImGui::Button("Load")) {
            // Open resource browser
            ImGui::BeginPopup("Gegg");

            ImGui::EndPopup();
        }
    }
}

void PropertyDrawUtility::operator()(PropertyView& prop, StrongResourceRef& val)
{
    auto res = val.Get<Resource>();
    std::string title = "Invalid Resource";
    if (res) title = res->GetName();
    
    ImGui::Text("%s", title.c_str());
    if (ImGui::BeginDragDropTarget()) {
        auto payload = ImGui::AcceptDragDropPayload("RESOURCE_PATH");
        if (payload)
        {
            ResourcePathDragDropData* data = static_cast<ResourcePathDragDropData*>(payload->Data);
            auto res_ref = ResourceManager::Load(data->GetPath());

            if (res_ref->GetStaticClassFromThis().IsSubclassOf(MeshResource::GetStaticClass()))
            {
                val = res_ref;
                prop.set(val);
            }
        }
        
        ImGui::EndDragDropTarget();
    }
    
    if (ImGui::Button("Load")) {
        ImGui::OpenPopup("ResourceSelector");
    }
    
    if (ImGui::BeginPopup("ResourceSelector"))
    {
        int i = 0;
        for (auto& r : ResourceManager::GetAllResources())
        {
            if (r->GetStaticClassFromThis().IsSubclassOf(MeshResource::GetStaticClass()))
            {
                if (ImGui::Button((r->GetName() + "##" + std::to_string(i)).c_str()))
                {
                    val = r;
                    prop.set<StrongResourceRef>(val);
                }
                
                i++;
            }
        }
        ImGui::EndPopup();
    }
    
    
}
