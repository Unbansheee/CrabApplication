#pragma once
#include "Core/SceneTree.h"
#include "Nodes/Node.h"

class NodeEditorSceneRoot : public Node
{
public:
    virtual void Ready() override;

    SceneTree& GetSubtree() { return EditorTree; }
    
private:
    SceneTree EditorTree;
};
