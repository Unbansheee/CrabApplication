
import Engine.Application;
import Engine.Node.ImGuiContextWindow;
import Engine.Filesystem;
import Editor.Node.EditorUI;
import PlatformFolders;
import Engine.Resource.ResourceManager;
import Engine.Resource.Scene;
import Engine.Node.Camera3D;

#define FORCE_EDITOR_ON 0

int main (int argc, char** argv) {
    bool launchEditor = false;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "--editor")
        {
            launchEditor = true;
        }
    }

    auto& app = Application::Get();
    Filesystem::AddFileSystemDirectory("/res", RESOURCE_DIR);
    Filesystem::AddFileSystemDirectory("/appdata", sago::getDataHome()+"/CrabApplication/");

    app.GetScriptEngine()->LoadModule(Filesystem::StringToWString(Filesystem::AbsolutePath("/dotnet/CrabApplication.dll")), L"CrabApplication");
    app.GetScriptEngine()->LoadModule(Filesystem::StringToWString(Filesystem::AbsolutePath("/dotnet/CrabModule.dll")), L"CrabModule");

    auto startScene = ResourceManager::Load<SceneResource>("/res/SolarSystem.scene");
    NodeWindow* window = nullptr;

    if (launchEditor || FORCE_EDITOR_ON) {
        window = app.GetSceneTree().SetRoot(Node::NewNode<NodeImGUIContextWindow>("Crab Editor"));
        window->SetSurfaceDrawEnabled(false);
        window->AddChild<NodeEditorUI>("EditorUI");
        app.GetSceneTree().SetUsePhysics(false);
    }
    else {
        window = app.GetSceneTree().SetRoot(Node::NewNode<NodeWindow>("Crab Application"));
        window->AddChild(startScene->Instantiate());
    }

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
