#pragma once
#include "Node.h"

class NodeEditorUI : public Node
{
public:
    NodeEditorUI() : Node() {}
    NodeEditorUI(const std::string name) : Node(name) {}

protected:
    void Begin() override;
    void Update(float dt) override;
    void DrawGUI() override;

public:
    float frametime = 0.0f;
    ~NodeEditorUI() override;
};
