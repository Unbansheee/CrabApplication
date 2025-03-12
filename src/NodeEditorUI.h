#pragma once
#include "NodeEditorCamera3D.h"
#include "NodeEditorSceneRoot.h"
#include "WeakRef.h"
#include "Nodes/Node.h"
#include "NodeSceneTreeUI.h"

class NodeSceneTreeUI;

class NodeEditorUI : public Node
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
};
