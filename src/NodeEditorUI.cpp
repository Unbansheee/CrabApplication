#include "NodeEditorUI.h"

#include "Application.h"
#include "imgui.h"
#include "NodeInspectorUI.h"
#include "NodeSceneTreeUI.h"
#include "NodeViewportUI.h"
#include "SceneSerializer.h"

void NodeEditorUI::Init()
{
    Node::Init();
    auto tree = AddChild<NodeSceneTreeUI>("Editor Tree");
    auto inspector = AddChild<NodeInspectorUI>("Editor Inspector");
    auto viewport = AddChild<NodeViewportUI>("Viewport");

    EditorSceneTree = AddChild<NodeSceneTreeUI>("Scene Tree");
    EditorRoot = AddChild<NodeEditorSceneRoot>("Scene Root");
    EditorCamera = AddChild<NodeEditorCamera3D>("Editor Camera");
    EditorSceneTree->SceneRootOverride = EditorRoot->GetSubtree().GetRoot<Node>();

    EditorSceneTree->OnNodeSelected.connect(inspector, &NodeInspectorUI::SetViewedNode);
    EditorSceneTree->OnNodeSelected.connect(viewport, &NodeViewportUI::SetViewedNode);
    tree->OnNodeSelected.connect(inspector, &NodeInspectorUI::SetViewedNode);
    EditorSceneTree->OnNodeSelected.connect([this](Node* newNode)
    {
        SelectedNode = newNode;
    });
    tree->OnNodeSelected.connect([this](Node* newNode)
    {
        SelectedNode = newNode;
    });
    //tree->OnNodeSelected.connect(viewport, &NodeViewportUI::SetViewedNode);
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
                OpenScene(ENGINE_RESOURCE_DIR"/TestScene.json");
            }

            if (ImGui::MenuItem("Save Scene"))
            {
                SaveScene(ENGINE_RESOURCE_DIR"/TestScene.json");
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

    ImGui::Begin("Add Nodes");

    auto classes = ClassDB::Get().GetSubclassesOf<Node>();
    for (auto& t : classes)
    {
        if (!(t.Flags & (uint32_t)ClassFlags::EditorVisible)) continue;
        if (ImGui::Button(t.Name.c_str()))
        {
            Object* n = t.Initializer();
            Node* node = dynamic_cast<Node*>(n);
            auto instance = Node::InitializeNode(node, t.Name);
            if (SelectedNode)
            {
                SelectedNode->AddChild(std::move(instance));
            }
            else
            {
                Application::Get().GetSceneTree().GetRoot<Node>()->AddChild(std::move(instance));
            }
        }
    }
    
    ImGui::End();

    ImGui::Begin("Add Resources");
    /*
    auto resources = ClassDB::Get().GetSubclassesOf<Resource>();
    for (auto& t : resources)
    {
        if (!(t.Flags & (uint32_t)ClassFlags::EditorVisible)) continue;
        if (ImGui::Button(t.Name.c_str()))
        {
            Object* n = t.Initializer();
            Node* node = dynamic_cast<Node*>(n);
            auto instance = Node::InitializeNode(node, t.Name);
            if (SelectedNode)
            {
                SelectedNode->AddChild(std::move(instance));
            }
            else
            {
                Application::Get().GetSceneTree().GetRoot<Node>()->AddChild(std::move(instance));
            }
        }
    }
    */
    
    ImGui::End();
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
    ser.SerializeScene(EditorRoot->GetSubtree().GetRoot<Node>(), ENGINE_RESOURCE_DIR"/TestScene.json");
}


NodeEditorUI::~NodeEditorUI()
{
}
