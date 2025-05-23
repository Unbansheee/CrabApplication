﻿module node_editor_scene_root;
import Engine.GFX.Renderer;
import Engine.Node.Window;
void NodeEditorSceneRoot::Init()
{
    Node::Init();
    EditorTree.bIsInEditor = true;
    EditorTree.SetRoot(NewNode("Root"));
    RootChanged.invoke();
}

void NodeEditorSceneRoot::Ready()
{
    Node::Ready();
}


void NodeEditorSceneRoot::Stop() {
    bIsActive = false;
    EditorTree.bIsInEditor = true;
    if (snapshot) {
        EditorTree.SetRoot(std::move(snapshot));
    }
    else {
        EditorTree.SetRoot(NewNode("Root"));
    }
    Stopped.invoke();
    RootChanged.invoke();
}

void NodeEditorSceneRoot::Run() {
    // Create a duplicate of the current scene and swap it in
    EditorTree.bIsInEditor = false;

    auto newRoot = EditorTree.GetRoot<Node>()->Duplicate();
    snapshot = EditorTree.SwapRoot(std::move(newRoot));

    bIsActive = true;
    EditorTree.Begin();
    Started.invoke();
}


void NodeEditorSceneRoot::Update(float dt)
{
    Node::Update(dt);
    if (bIsActive)
    {
        EditorTree.Update(dt);
    }
}

InputResult NodeEditorSceneRoot::HandleInput(const InputEvent &event) {
    if (bIsActive) {
        NodeWindow::PropagateInputToChildren(GetSubtree().GetRoot(), event);
    }
    return Node::HandleInput(event);
}

void NodeEditorSceneRoot::Render(Renderer& Visitor)
{
    Node::Render(Visitor);
    Visitor.GatherDrawCommands(EditorTree.GetRoot<Node>());
}
