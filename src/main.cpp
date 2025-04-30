
import Engine.Application;
import Engine.Node;
import Engine.Node.ImGuiContextWindow;
import Engine.Filesystem;

import Editor.Node.EditorUI;
import Engine.ScriptEngine;

int main (int, char**) {
    auto n = Node::NewNode<Node>();
    auto& app = Application::Get();
    Filesystem::AddFileSystemDirectory("/res", RESOURCE_DIR);

    auto window = app.GetSceneTree().SetRoot(Node::NewNode<NodeImGUIContextWindow>("Crab Editor"));
    window->SetSurfaceDrawEnabled(false);
    window->AddChild<NodeEditorUI>("EditorUI");
    app.GetSceneTree().SetUsePhysics(false);

    app.Begin();



    while (!app.ShouldClose())
    {
        if (window->WantsToClose())
        {
            app.Close();
        }
        app.Update();
    }
    
    return 0;
}
