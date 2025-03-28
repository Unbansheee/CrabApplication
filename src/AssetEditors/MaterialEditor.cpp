//
// Created by Vinnie on 28/03/2025.
//
module;
#include "imgui.h"

module Editor.MaterialEditor;
import Engine.PropertyDraw;

void MaterialEditor::Update(float dt) {
    Node::Update(dt);
}

void MaterialEditor::DrawGUI() {
    Node::DrawGUI();

    bool open = true;
    ImGui::Begin("Material Editor", &open);
    if (Context) {
        PropertyDrawUtility p;
        for (auto prop : Context->GetPropertiesFromThis()) {
            prop.visit(p, Context.get());
        }
    }
    ImGui::End();

    if (!open) RemoveFromParent();
}

void MaterialEditor::Ready() {
    Node::Ready();
}
