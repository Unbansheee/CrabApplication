//
// Created by Vinnie on 24/04/2025.
//


module;
#include "ReflectionMacros.h"

export module Editor.SceneFileEditor;
import Editor.AssetEditorRegistry;
import Engine.Node;
import Engine.Resource;
import Editor.AssetEditor;
import Engine.Resource.Scene;
import node_editor_ui;
import Engine.Assert;

export class SceneFileEditor : public AssetEditor{
    CRAB_CLASS(SceneFileEditor, AssetEditor);

    inline static AssetEditorAutoRegister<SceneFileEditor, SceneResource> AutoReg;

public:
    void Ready() override;

};

void SceneFileEditor::Ready() {
    AssetEditor::Ready();

    auto rootNode = GetAncestorOfType<NodeEditorUI>();
    rootNode->NewScene();
    try {
        rootNode->EditorRoot->GetSubtree().SetRoot(std::static_pointer_cast<SceneResource>(Context)->Instantiate());
        rootNode->EditorSceneTree->SceneRootOverride = rootNode->EditorRoot->GetSubtree().GetRoot<Node>();
    } catch (std::exception e) {
        Assert::Fail("Error opening Scene, " + std::string(e.what()));
    }


    RemoveFromParent();
}
