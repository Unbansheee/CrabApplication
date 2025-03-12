#include "NodeEditorSceneRoot.h"

void NodeEditorSceneRoot::Init()
{
    Node::Init();
    EditorTree.SetRoot(NewNode("Root"));
}

void NodeEditorSceneRoot::Ready()
{
    Node::Ready();
    EditorTree.Begin();
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
