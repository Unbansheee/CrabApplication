#pragma once
#include "Nodes/Node.h"

class NodeEditorUI : public Node
{
public:
    void Init() override;

protected:
    void Update(float dt) override;
    void DrawGUI() override;

public:
    float frametime = 0.0f;
    ~NodeEditorUI() override;
};
