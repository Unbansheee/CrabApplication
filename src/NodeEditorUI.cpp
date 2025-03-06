#include "NodeEditorUI.h"

#include "imgui.h"
#include "NodeInspectorUI.h"
#include "NodeSceneTreeUI.h"
#include "NodeViewportUI.h"

void NodeEditorUI::Begin()
{
    Node::Begin();
    auto tree = AddChild<NodeSceneTreeUI>("SceneTreePanel");
    auto inspector = AddChild<NodeInspectorUI>("InspectorPanel");
    auto viewport = AddChild<NodeViewportUI>("Viewport");
    
    tree->OnNodeSelected.connect(inspector, &NodeInspectorUI::SetViewedNode);
    tree->OnNodeSelected.connect(viewport, &NodeViewportUI::SetViewedNode);
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
        ImGui::MenuItem("File");
        ImGui::MenuItem("Edit");
        ImGui::EndMainMenuBar();
    }

    ImGui::Begin("Stats");
    ImGui::Text("FPS: %f", ImGui::GetIO().Framerate);
    ImGui::Text("Frametime: %f", frametime * 1000.f);
    ImGui::End();
}

NodeEditorUI::~NodeEditorUI()
{
}
