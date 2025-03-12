#include "NodeEditorSceneRoot.h"

void NodeEditorSceneRoot::Ready()
{
    Node::Ready();
    EditorTree.Begin();
}
