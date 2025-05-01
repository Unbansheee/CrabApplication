//
// Created by Vinnie on 2/05/2025.
//

module Editor.SceneFileEditor;

void SceneFileEditor::Ready() {
    AssetEditor::Ready();

    auto rootNode = GetAncestorOfType<NodeEditorUI>();
    rootNode->NewScene();
    rootNode->EditorRoot->GetSubtree().SetRoot(std::static_pointer_cast<SceneResource>(Context)->Instantiate());
    rootNode->EditorSceneTree->SceneRootOverride = rootNode->EditorRoot->GetSubtree().GetRoot<Node>();

    RemoveFromParent();
}
