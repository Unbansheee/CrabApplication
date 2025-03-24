module node_editor_scene_root;

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

void NodeEditorSceneRoot::Render(RenderVisitor& Visitor)
{
    Node::Render(Visitor);
    EditorTree.GetRoot<Node>()->Render(Visitor);
}
