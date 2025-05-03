
import Engine.Application;
import Engine.Node.ImGuiContextWindow;
import Engine.Filesystem;
import Editor.Node.EditorUI;
import PlatformFolders;

int main (int, char**) {
    auto& app = Application::Get();
    Filesystem::AddFileSystemDirectory("/res", RESOURCE_DIR);
    Filesystem::AddFileSystemDirectory("/appdata", sago::getDataHome()+"/CrabApplication/");

    app.GetScriptEngine()->LoadModule(Filesystem::StringToWString(Filesystem::AbsolutePath("/dotnet/CrabApplication.dll")), L"CrabApplication");
    app.GetScriptEngine()->LoadModule(Filesystem::StringToWString(Filesystem::AbsolutePath("/dotnet/CrabModule.dll")), L"CrabModule");


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
