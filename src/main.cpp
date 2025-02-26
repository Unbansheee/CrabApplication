#include <iostream>

#include "Application.h"
#include "ResourceManager.h"
#include "TestMaterial.h"
#include "Nodes/NodeImGUIContextWindow.h"
#include "Nodes/NodeWindow.h"
#include "NodeEditorUI.h"
#include "Gfx/Materials/StandardMaterial.h"

int main (int, char**) {

    std::cout << "Hello World!" << std::endl;

    auto& app = Application::Get();
    auto window = app.SetRootNode(std::make_unique<NodeImGUIContextWindow>("Crab Editor"));
    window->SetSurfaceDrawEnabled(false);
    window->AddChild<NodeEditorUI>("EditorUI");
    app.Begin();
    
    auto meshInst = window->AddChild<NodeMeshInstance3D>("Mesh");
    meshInst->SetPosition({0, 0, 0});
    meshInst->SetOrientation(Quat({0.0, 0.0, 90.f}));
    
    auto view = glm::lookAt(glm::vec3(4.0f, 0.f, 0.0f), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
    auto proj = glm::perspective(45 * PI / 180, 640.f/480.f, 0.01f, 1000.0f);
    
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
    
    
    meshInst->SetMesh(m);
    meshInst->SetMaterial(mat);
    
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
