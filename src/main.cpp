#include <iostream>

#include "ReflectionMacros.h"
/*
#include "Application.h"
#include "GLTFSceneParser.h"
#include "NodeEditorSceneRoot.h"
#include "Resource/ResourceManager.h"
#include "Nodes/NodeImGUIContextWindow.h"
#include "Nodes/NodeWindow.h"
#include "NodeEditorUI.h"
#include "Resource/OBJMeshResource.h"
#include "Resource/Importers/TextureImporter.h"
#include "Resource/Importers/OBJMeshImporter.h"
*/

import application;
import node;
import node_imgui_context_window;
import node_editor_ui;
import resource_manager;
import obj_mesh_resource;
import resource_ref;
import obj_mesh_importer;

StrongResourceRef r;
REGISTER_RESOURCE_IMPORTER(OBJMeshImporter)
int main (int, char**) {
    auto n = Node::NewNode<Node>();
    auto& app = Application::Get();
    auto window = app.GetSceneTree().SetRoot(Node::NewNode<NodeImGUIContextWindow>("Crab Editor"));
    window->SetSurfaceDrawEnabled(false);
    window->AddChild<NodeEditorUI>("EditorUI");
    
    app.Begin();

    
    
    //r = ResourceManager::Load<OBJMeshResource>(ENGINE_RESOURCE_DIR"/fourareen.obj");
    
    
    /*
    auto meshInst = window->AddChild<NodeMeshInstance3D>("Mesh");
    meshInst->SetPosition({0, 0, 0});
    meshInst->SetOrientation(Quat(glm::radians(glm::vec3{0.0, 0.0, 90.f})));
    
    auto meshInst2 = meshInst->AddChild<NodeMeshInstance3D>("Child Mesh");
    meshInst2->SetPosition({0, 0, 1});

    auto gltfParser = GLTFSceneParser();
    window->AddChild(gltfParser.ParseGLTF(app.GetDevice(), ENGINE_RESOURCE_DIR"/Level1.glb"));
    
    std::vector<MeshVertex> v;
    ResourceManager::loadGeometryFromObj(ENGINE_RESOURCE_DIR "/fourareen.obj", v);
    SharedRef<Mesh> m = MakeShared<Mesh>(app.GetDevice(), v, std::nullopt);
    auto norm = ResourceManager::Load<ImageTextureResource>(ENGINE_RESOURCE_DIR"/fourareen2K_normals.png");
    auto albedo = ResourceManager::Load<ImageTextureResource>(ENGINE_RESOURCE_DIR"/fourareen2K_albedo.jpg");
    norm->imageFilePath = norm->GetResourcePath();
    albedo->imageFilePath = albedo->GetResourcePath();
    norm->LoadData();
    albedo->LoadData();

    //SharedRef<TextureResource> normal = Resource::CreateResource<TextureResource>(ENGINE_RESOURCE_DIR"/fourareen2K_normals.png");
    //SharedRef<TextureResource> albedo = Resource::CreateResource<TextureResource>(ENGINE_RESOURCE_DIR"/fourareen2K_albedo.jpg");
    SharedRef<StandardMaterial> mat = MakeShared<StandardMaterial>(app.GetDevice(), ENGINE_RESOURCE_DIR"/standard_material.wgsl");
    mat->TargetTextureFormat = window->GetSurfaceFormat();
    mat->NormalTextureView = norm;
    mat->BaseColorTextureView = albedo;
    mat->Initialize();
    
    meshInst->SetMesh(m);
    meshInst->SetMaterial(mat);
    meshInst2->SetMesh(m);
    meshInst2->SetMaterial(mat);
    */

    
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
