#include "NodeEditorUI.h"

#include "imgui.h"
#include "NodeInspectorUI.h"
#include "NodeSceneTreeUI.h"

void NodeEditorUI::Begin()
{
    Node::Begin();
    auto tree = AddChild<NodeSceneTreeUI>("SceneTreePanel");
    auto inspector = AddChild<NodeInspectorUI>("InspectorPanel");

    tree->OnNodeSelected.connect(inspector, &NodeInspectorUI::SetViewedNode);
}

void NodeEditorUI::Update(float dt)
{
    Node::Update(dt);
}

void NodeEditorUI::DrawGUI()
{
    Node::DrawGUI();
    ImGui::Begin("Editor");
    ImGui::End();
}

NodeEditorUI::~NodeEditorUI()
{
}
