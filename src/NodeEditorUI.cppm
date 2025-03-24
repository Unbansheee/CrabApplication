module;
#pragma once
#include "ReflectionMacros.h"

export module node_editor_ui;
import node_content_browser_panel;
import node_editor_camera_3d;
import node_editor_scene_root;
import node_scene_tree_ui;
import Engine.Object.Ref;
import Engine.Node;
import Engine.Reflection.AutoRegistration;

//export class NodeSceneTreeUI;

export class NodeEditorUI : public Node
{
public:
    CRAB_CLASS(NodeEditorUI, Node)
    BEGIN_PROPERTIES
    END_PROPERTIES

    void Init() override;

protected:
    void Update(float dt) override;
    void DrawGUI() override;
    void Ready() override;

    void OpenScene(const std::string& path);
    void NewScene();
    void SaveScene(const std::string& path);
    void ImportGLB(const std::string& path);
public:
    float frametime = 0.0f;
    ~NodeEditorUI() override;

    ObjectRef<Node> SelectedNode;
    ObjectRef<NodeEditorSceneRoot> EditorRoot;
    ObjectRef<NodeEditorCamera3D> EditorCamera;
    ObjectRef<NodeSceneTreeUI> EditorSceneTree;
    ObjectRef<NodeContentBrowserPanel> ContentBrowser;
};
