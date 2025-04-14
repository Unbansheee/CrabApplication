module;
#pragma once
#include "ReflectionMacros.h"

export module node_editor_scene_root;
import Engine.Node;
import Engine.SceneTree;

export class NodeEditorSceneRoot : public Node
{
public:
    CRAB_CLASS(NodeEditorSceneRoot, Node)

    virtual void Init() override;
    virtual void Ready() override;
    virtual void Update(float dt) override;

    void Run();
    void Stop();

    rocket::signal<void()> Started;
    rocket::signal<void()> Stopped;
    rocket::signal<void()> RootChanged;

    void Render(Renderer& Visitor) override;

    SceneTree& GetSubtree() { return EditorTree; }

    bool bIsActive{false};
    std::unique_ptr<Node> snapshot;
private:
    SceneTree EditorTree;
};
