#pragma once
#include "Core/SceneTree.h"
#include "Nodes/Node.h"

class NodeEditorSceneRoot : public Node
{
public:
    virtual void Init() override;
    virtual void Ready() override;
    virtual void Update(float dt) override;

    void Render(RenderVisitor& Visitor) override;
    
    SceneTree& GetSubtree() { return EditorTree; }

    bool bIsActive{false};
    
private:
    SceneTree EditorTree;
};
