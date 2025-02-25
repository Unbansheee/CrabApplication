#include <iostream>

#include "Application.h"
#include "ResourceManager.h"
#include "TestMaterial.h"
#include "Nodes/NodeImGUIContextWindow.h"
#include "Nodes/NodeWindow.h"
#include "NodeEditorUI.h"

int main (int, char**) {

    std::cout << "Hello World!" << std::endl;

    auto& app = Application::Get();
    auto window = app.SetRootNode(std::make_unique<NodeImGUIContextWindow>("Crab Editor"));
    window->AddChild<NodeEditorUI>("EditorUI");
    app.Begin();
    
    auto meshInst = window->AddChild<NodeMeshInstance3D>("Mesh");
    meshInst->SetPosition({0, 0, 0});
    meshInst->SetOrientation(Quat({0.0, 0.0, 90.f}));
    
    auto view = glm::lookAt(glm::vec3(4.0f, 0.f, 0.0f), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
    auto proj = glm::perspective(45 * PI / 180, 640.f/480.f, 0.01f, 1000.0f);
    
    std::vector<MeshVertex> v;
    ResourceManager::loadGeometryFromObj(ENGINE_RESOURCE_DIR "/fourareen.obj", v);
    std::shared_ptr<Mesh> m = std::make_shared<Mesh>(app.GetDevice(), v, std::nullopt);

    Application::MyUniforms testUniforms;
    std::shared_ptr<TestMaterial> mat = std::make_shared<TestMaterial>(app.GetDevice(), RESOURCE_DIR "/testShader.wgsl");
    mat->TargetTextureFormat = window->GetSurfaceFormat();
    mat->Initialize();
    
    Application::MyUniforms uniforms {};
    uniforms.projectionMatrix = proj;
    uniforms.viewMatrix = view;
    uniforms.color = {0.0, 0.0, 1.0, 1.0};
    uniforms.time = glfwGetTime();
    uniforms.cameraWorldPosition = glm::vec3(0.0f, 3.f, 0.0f);

    mat->uniformBuffer.SetData(uniforms);

    glm::mat4x4 modelMat = meshInst->GetTransform().GetWorldModelMatrix();
    mat->perObjectBuffer.SetData(modelMat);
    
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
