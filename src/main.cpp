#include <iostream>

#include "Application.h"
#include "ResourceManager.h"
#include "TestMaterial.h"
#include "Nodes/NodeImGUIContextWindow.h"
#include "Nodes/NodeWindow.h"
#include "NodeEditorUI.h"
#include "Core/ClassDB.h"
#include "Gfx/Materials/StandardMaterial.h"

int main (int, char**) {
    auto& app = Application::Get();
    auto window = app.SetRootNode(std::make_unique<NodeImGUIContextWindow>("Crab Editor"));
    window->SetSurfaceDrawEnabled(false);
    window->AddChild<NodeEditorUI>("EditorUI");
    app.Begin();
    
    auto meshInst = window->AddChild<NodeMeshInstance3D>("Mesh");
    meshInst->SetPosition({0, 0, 0});
    meshInst->SetOrientation(Quat(glm::radians(glm::vec3{0.0, 0.0, 90.f})));
    
    auto meshInst2 = meshInst->AddChild<NodeMeshInstance3D>("Child Mesh");
    meshInst2->SetPosition({0, 0, 1});
    
    std::vector<MeshVertex> v;
    ResourceManager::loadGeometryFromObj(ENGINE_RESOURCE_DIR "/fourareen.obj", v);
    SharedRef<Mesh> m = MakeShared<Mesh>(app.GetDevice(), v, std::nullopt);
    SharedRef<TextureResource> normal = MakeShared<TextureResource>(ENGINE_RESOURCE_DIR"/fourareen2K_normals.png");
    SharedRef<TextureResource> albedo = MakeShared<TextureResource>(ENGINE_RESOURCE_DIR"/fourareen2K_albedo.jpg");
    SharedRef<StandardMaterial> mat = MakeShared<StandardMaterial>(app.GetDevice(), ENGINE_RESOURCE_DIR"/standard_material.wgsl");
    mat->TargetTextureFormat = window->GetSurfaceFormat();
    mat->NormalTextureView = normal;
    mat->BaseColorTextureView = albedo;
    mat->Initialize();
    
    for (const Property& prop : meshInst->GetClassProperties())
    {
        std::cout << "Property: " << prop.name << " -> ";
        if (prop.is<bool>())
        {
            std::cout << prop.get<bool>(meshInst);
        }
        if (prop.is<std::string>())
        {
            std::cout << prop.get<std::string>(meshInst);
        }
        std::cout << "\n";
    }
    
    meshInst->SetMesh(m);
    meshInst->SetMaterial(mat);
    meshInst2->SetMesh(m);
    meshInst2->SetMaterial(mat);
    
    while (!app.ShouldClose())
    {
        if (window->WantsToClose())
        {
            app.Close();
        }
        app.Update();
    }
    
    return 0;
}
