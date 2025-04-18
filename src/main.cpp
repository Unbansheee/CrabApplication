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

import Engine.Application;
import Engine.Node;
import Engine.Node.ImGuiContextWindow;
import node_editor_ui;
import Engine.Resource.ResourceManager;
import Engine.Resource.OBJMesh;
import Engine.Resource.Ref;
import Engine.ShaderCompiler;

import std;
import vfspp;

int main (int, char**) {
    auto n = Node::NewNode<Node>();
    auto& app = Application::Get();

    app.AddFileSystem("/res", RESOURCE_DIR);
    auto window = app.GetSceneTree().SetRoot(Node::NewNode<NodeImGUIContextWindow>("Crab Editor"));
    window->SetSurfaceDrawEnabled(false);
    window->AddChild<NodeEditorUI>("EditorUI");
    app.GetSceneTree().SetUsePhysics(false);

    app.Begin();
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
