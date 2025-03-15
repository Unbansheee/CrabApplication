#pragma once

export module node_editor_ui;
import node_content_browser_panel;
import node_editor_camera_3d;
import node_editor_scene_root;
import node_scene_tree_ui;
import object_ref;
import node;

//export class NodeSceneTreeUI;

export class NodeEditorUI : public Node
{
public:
    void Init() override;

protected:
    void Update(float dt) override;
    void DrawGUI() override;

    void OpenScene(const std::string& path);
    void NewScene();
    void SaveScene(const std::string& path);
public:
    float frametime = 0.0f;
    ~NodeEditorUI() override;

    WeakRef<Node> SelectedNode;
    WeakRef<NodeEditorSceneRoot> EditorRoot;
    WeakRef<NodeEditorCamera3D> EditorCamera;
    WeakRef<NodeSceneTreeUI> EditorSceneTree;
    WeakRef<NodeContentBrowserPanel> ContentBrowser;
};
