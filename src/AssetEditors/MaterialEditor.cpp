//
// Created by Vinnie on 28/03/2025.
//
module;
#include "imgui.h"

module Editor.MaterialEditor;
import Engine.PropertyDraw;
import Engine.WGPU;
import Engine.ShaderCompiler.Types;
import Engine.Resource.Texture;
import node_content_browser_panel;
import Engine.Resource.ResourceManager;
import Engine.Application;

void MaterialEditor::Update(float dt) {
    Node::Update(dt);
}

void MaterialEditor::DrawGUI() {
    Node::DrawGUI();

    bool open = true;
    ImGui::Begin("Material Editor", &open);
    if (Context) {
        if (ImGui::Button("Save")) {
            ResourceManager::SaveResource(Context);
        }


        if (ImGui::Button("Recompile Shader")) {
            std::static_pointer_cast<MaterialResource>(Context)->LoadFromShaderPath(Application::Get().GetDevice(), "ghghg");
        }


        PropertyDrawUtility p;
        for (auto prop : Context->GetPropertiesFromThis()) {
            prop.visit(p, Context.get());
        }

        auto mat = std::static_pointer_cast<MaterialResource>(Context);
        auto& uniformMetadata = mat->m_uniformMetadata;
        for (auto& uniform : uniformMetadata) {
            if (uniform.second.IsPushConstant) continue;
            if (!uniform.second.IsSerialized) continue;
            ImGui::Text(uniform.first.c_str());

            if (uniform.second.BindingType == Buffer) {
                const auto& bigBuffer = mat->GetUniformData(uniform.first);
                for (auto& field : uniform.second.BufferFields) {
                    if (field.Type == UniformMetadata::BufferField::FieldType::FLOAT) {
                        float val = *reinterpret_cast<const float*>(bigBuffer.data() + field.Offset);
                        if (ImGui::DragFloat(field.Name.c_str(), &val))
                        {
                            mat->SetUniformData(uniform.first, &val, field.SizeInBytes, field.Offset);
                        }
                    }
                }
            }

            else if (uniform.second.BindingType == Texture) {
                auto res = mat->GetTexture(uniform.first);
                std::string title = "Invalid Resource";
                if (res) title = res->GetName();

                ImGui::PushID(uniform.first.c_str());
                ImGui::Text(uniform.first.c_str());
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
                        auto res_ref = ResourceManager::Load<TextureResource>(data->GetPath());
                        mat->SetTexture(uniform.first, res_ref);
                    }

                    ImGui::EndDragDropTarget();
                }

                if (nodeOpen)
                {
                    if (res)
                    {
                        PropertyDrawUtility u;
                        for (auto p : res->GetPropertiesFromThis())
                        {
                            if ((uint32_t)p.flags & (uint32_t)PropertyFlags::HideFromInspector) continue;
                            p.visit(u, res.get());
                        }
                    }
                    ImGui::TreePop();
                }


                if (ImGui::BeginPopup("ResourceSelector"))
                {
                    int i = 0;
                    for (auto& r : ResourceManager::GetAllResources())
                    {
                        if (!r->IsA<TextureResource>()) continue;
                        if (ImGui::Button((r->GetName() + "##" + std::to_string(i)).c_str()))
                        {
                            mat->SetTexture(uniform.first, std::static_pointer_cast<TextureResource>(r));
                            //prop.set<StrongResourceRef>(val);
                        }

                        i++;

                    }
                    ImGui::EndPopup();
                }
                ImGui::PopID();

            }

        }
    }
    ImGui::End();

    if (!open) RemoveFromParent();
}

void MaterialEditor::Ready() {
    Node::Ready();
}
