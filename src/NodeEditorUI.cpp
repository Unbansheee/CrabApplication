module;
#include "imgui.h"
#include "nativefiledialog-extended/src/include/nfd.h"
#include "nativefiledialog-extended/src/include/nfd.hpp"
module node_editor_ui;
import Engine.Application;
import node_inspector_ui;
import node_scene_tree_ui;
import node_viewport_ui;
import Engine.SceneSerializer;
import Engine.Node;
import editor_theme;
import Engine.GLTFSceneParser;
import Engine.Resource.RuntimeTexture;
import Engine.Resource.OBJMesh;
import Engine.Resource.ResourceManager;

void NodeEditorUI::Init()
{
    Node::Init();
    auto editorTree = AddChild<NodeSceneTreeUI>("Editor Tree");
    auto inspector = AddChild<NodeInspectorUI>("Editor Inspector");
    auto viewport = AddChild<NodeViewportUI>("Viewport");

    ContentBrowser = AddChild<NodeContentBrowserPanel>("Content Browser");
    EditorSceneTree = AddChild<NodeSceneTreeUI>("Scene Tree");
    EditorRoot = AddChild<NodeEditorSceneRoot>("Scene Root");
    EditorCamera = AddChild<NodeEditorCamera3D>("Editor Camera");
    EditorSceneTree->SceneRootOverride = EditorRoot->GetSubtree().GetRoot<Node>();

    viewport->OnNodeSelectedInViewport.connect(inspector, &NodeInspectorUI::SetViewedNode);
    viewport->OnNodeSelectedInViewport.connect([this](Node* newNode)
    {
        SelectNode(newNode);
    });

    SelectedNodeChanged.connect([viewport, inspector](Node* node)
    {
        viewport->selectedNode = node;
        inspector->SetViewedNode(node);
    });
    //tree->OnNodeSelected.connect(viewport, &NodeViewportUI::SetViewedNode);

    PlayButtonTexture = ResourceManager::Load<TextureResource>(ENGINE_RESOURCE_DIR"/Textures/T_PlayButton.png");
    StopButtonTexture = ResourceManager::Load<TextureResource>(ENGINE_RESOURCE_DIR"/Textures/T_StopButton.png");

}

void NodeEditorUI::Update(float dt)
{
    Node::Update(dt);
    frametime = dt;
}

void NodeEditorUI::DrawGUI()
{
    Node::DrawGUI();

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Scene"))
            {
                NewScene();
            }

            if (ImGui::MenuItem("Open Scene"))
            {
                nfdu8char_t *outPath;
                nfdu8filteritem_t filters[1] = { { "Scene", "scene" }};
                nfdresult_t result = NFD::OpenDialog(outPath, filters, 1, RESOURCE_DIR);
                if (result == NFD_OKAY)
                {
                    OpenScene(outPath);
                    NFD_FreePathU8(outPath);
                }
            }

            if (ImGui::MenuItem("Save Scene"))
            {
                
                nfdu8char_t *outPath;
                nfdu8filteritem_t filters[1] = { { "Scene", "scene" }};
                nfdresult_t result = NFD::SaveDialog(outPath, filters, 1, RESOURCE_DIR);
                if (result == NFD_OKAY)
                {
                    SaveScene(outPath);
                    NFD_FreePathU8(outPath);
                }
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Import GLB"))
            {
                nfdu8char_t *outPath;
                nfdu8filteritem_t filters[1] = { { "GLB", "glb" }};
                nfdresult_t result = NFD::OpenDialog(outPath, filters, 1, RESOURCE_DIR);
                if (result == NFD_OKAY)
                {
                    ImportGLB(outPath);
                    NFD_FreePathU8(outPath);
                }
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Quit"))
            {
                Application::Get().Close();
            }
            
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            
            ImGui::EndMenu();
        };
        ImGui::EndMainMenuBar();
    }

    ImGui::Begin("Stats");
    ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
    ImGui::Text("Frametime: %f", frametime * 1000.f);
    ImGui::End();


    ImGui::Begin("Toolbar");
    if (!EditorRoot->bIsActive) {

        if (ImGui::ImageButton("PlayButton", (ImTextureID)(WGPUTextureView)PlayButtonTexture->GetInternalTextureView(), ImVec2(32, 32))) {
            EditorRoot->Run();
        }
    }
    else {
        if (ImGui::ImageButton("StopButton", (ImTextureID)(WGPUTextureView)StopButtonTexture->GetInternalTextureView(), ImVec2(32, 32))) {
            EditorRoot->Stop();
        }
    }

    ImGui::End();
    
}

void NodeEditorUI::Ready()
{
    Node::Ready();
    ThemeManager::LoadTheme("Dark");
    ThemeManager::SetDefaultFont(ENGINE_RESOURCE_DIR"/Fonts/Roboto-Light.ttf");
}

void NodeEditorUI::OpenScene(const std::string& path)
{
    NewScene();
    
    SceneSerializer ser;
    ser.DeserializeScene(EditorRoot->GetSubtree().GetRoot<Node>(), path);
}

void NodeEditorUI::NewScene()
{
    EditorRoot->RemoveFromParent();
    EditorRoot = AddChild<NodeEditorSceneRoot>("Scene Root");
    EditorSceneTree->SceneRootOverride = EditorRoot->GetSubtree().GetRoot<Node>();
    std::cout << "Reset Scene\n";
}

void NodeEditorUI::SaveScene(const std::string& path)
{
    SceneSerializer ser;
    ser.SerializeScene(EditorRoot->GetSubtree().GetRoot<Node>(), path);
}

void NodeEditorUI::ImportGLB(const std::string &path) {
    GLTFSceneParser parser;
    auto gltf = parser.ParseGLTF(Application::Get().GetDevice(), path);
    if (SelectedNode) {
        SelectedNode->AddChild(std::move(gltf));
    }
    else {
        EditorSceneTree->GetRootNode()->AddChild(std::move(gltf));
    }
}


NodeEditorUI::~NodeEditorUI()
{
}
