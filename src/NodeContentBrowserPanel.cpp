module;
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

module node_content_browser_panel;
import Engine.Resource.ResourceManager;
import Engine.WGPU;
import Engine.Resource.ImageTexture;
import Editor.AssetEditorRegistry;
import Editor.AssetEditor;
import Engine.Application;
import Engine.Filesystem;

namespace fs = std::filesystem;
void sort_directory_entries(std::vector<fs::directory_entry>& entries) {
    std::sort(entries.begin(), entries.end(),
        [](const fs::directory_entry& a, const fs::directory_entry& b) {
            bool a_is_dir = a.is_directory();
            bool b_is_dir = b.is_directory();
            if (a_is_dir != b_is_dir) {
                // Directories come before files
                return a_is_dir;
            } else {
                // Compare filenames alphabetically
                return a.path().filename().string() < b.path().filename().string();
            }
        });
}


void NodeContentBrowserPanel::Init()
{
    Node::Init();
    FolderTexture = ResourceManager::Load<TextureResource>("/engine/Textures/T_FolderThumbnail.png");
    SetCurrentDirectory(RESOURCE_DIR);
}

void NodeContentBrowserPanel::Ready()
{
    Node::Ready();
}

void NodeContentBrowserPanel::Update(float dt)
{
    Node::Update(dt);

    auto dirIter = std::filesystem::directory_iterator(currentDirectory);
    int fileCount = std::count_if(
        begin(dirIter),
        end(dirIter),
        [](auto& entry) { return entry.path().extension() != ".meta"; }
    );

    if (fileCount != currentEntries.size()) {
        RefreshBrowser();
    }
}

void NodeContentBrowserPanel::DrawGUI()
{
    Node::DrawGUI();
    ImGui::Begin("Content Browser");
    if (ImGui::Button("<-"))
    {
        bool found = false;
        for (auto& root : rootDirectories) {
            if (root.AbsolutePath == currentDirectory) {
                found = true;
                break;
            }
        }
        if (!found) {
            SetCurrentDirectory(currentDirectory.parent_path());
        }
    }
    ImGui::SameLine();
    
    ImGui::Text(currentDirectory.string().c_str());

    ImGui::SameLine(ImGui::GetWindowSize().x - ImGui::CalcTextSize("Create Resource").x - 30);
    if (ImGui::Button("Create Resource")) {
        ImGui::OpenPopup("ResourceCreationPopup");
    }
    DrawResourceCreationMenu();

    ImGui::Separator();
    
    //ImGui::Columns(2);
    ImGui::BeginChild("SourcesPanel", {100, 0}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AlwaysAutoResize);
    int _rootSelectIndex = 0;
    for (auto& root : rootDirectories)
    {
        if (ImGui::ButtonEx(root.Alias.c_str()))
        {
            currentRootIndex = _rootSelectIndex;
            SetCurrentDirectory(root.AbsolutePath);
        }
        _rootSelectIndex++;
    }
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);

    ImGui::SameLine();
    ImGui::BeginChild("BrowserPanel");
    float panelWidth = ImGui::GetContentRegionAvail().x;
    float cellSize = padding + itemSize;
    int columnCount = (int)(panelWidth / cellSize);
    columnCount = std::max(columnCount, 1);


    ImGui::Columns(columnCount, 0, false);

    for (auto& i : currentEntries)
    {
        auto reply = DrawAssetWidget(i);
        ImGui::NextColumn();
        if (reply == ReconstructingAssetPanel) break;
    }


    
    ImGui::Columns(1);
    ImGui::EndChild();
    ImGui::Columns(1);

    ImGui::End();
}

NodeContentBrowserPanel::EReply NodeContentBrowserPanel::DrawAssetWidget(const ContentBrowserEntry& entry)
{
    wgpu::raii::TextureView tex{};
    if (entry.Thumbnail.has_value()) {
        tex = entry.Thumbnail.value();
    }
    std::string path = entry.DirectoryEntry.path().filename().string();

    if (tex) {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::ImageButton(("##" + path).c_str(), (uint64_t)(WGPUTextureView)*tex, {itemSize, itemSize});
        ImGui::PopStyleVar();
    }
    else {
        ImGui::Button(("##" + path).c_str(), {itemSize, itemSize});
    }

    if (ImGui::BeginDragDropSource())
    {
        ResourcePathDragDropData data(entry.DirectoryEntry.path().string());
        ImGui::SetDragDropPayload("RESOURCE_PATH", &data, sizeof(ResourcePathDragDropData));
        ImGui::EndDragDropSource();
    }
    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
    {
        if (entry.DirectoryEntry.is_directory())
        {
            SetCurrentDirectory(currentDirectory / entry.DirectoryEntry.path().filename());
            return ReconstructingAssetPanel;
        }
        else {
            auto relPath = rootDirectories[currentRootIndex].Alias + "/" + std::filesystem::relative(entry.DirectoryEntry.path(), rootDirectories[currentRootIndex].AbsolutePath).string();

            if (ResourceManager::IsSourceFile(relPath)) {
                if (auto res = ResourceManager::Load(relPath)) {
                    if (auto editor = AssetEditorRegistry::CreateEditorFor(res)) {
                        static_cast<AssetEditor*>(editor.get())->SetContext(res);
                        GetParent()->AddChild(std::move(editor));
                    }
                }
            }
        }
    }
    ImGui::TextWrapped(path.c_str());
    return Success;
}

std::string resName = std::string("NewResource");
void NodeContentBrowserPanel::DrawResourceCreationMenu() {
    if (ImGui::BeginPopup("ResourceCreationPopup")) {
        for (auto& res : GetAvailableResourceTypes()) {

            if (ImGui::BeginPopupModal("Resource Configurator")) {

                ImGui::InputText("Resource Name", &resName);

                if (ImGui::Button("Create")) {
                    auto instance = static_cast<Resource*>(res->Initializer());
                    std::shared_ptr<Resource> ptr;
                    ptr.reset(instance);
                    std::string outPath = currentDirectory.string() + "/" + resName + ".res";
                    ResourceManager::SaveResource(ptr, outPath);
                    ResourceManager::Load(outPath);
                    ImGui::CloseCurrentPopup();
                    RefreshBrowser();
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel")) {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }

            if (ImGui::Button(res->Name.string())) {
                resName = std::string("New") + res->Name.string();
                ImGui::OpenPopup("Resource Configurator");
            }
        }
        ImGui::EndPopup();
    }
}

std::vector<const ClassType*> NodeContentBrowserPanel::GetAvailableResourceTypes() {
    const auto& types = ClassDB::Get().GetClasses();
    std::vector<const ClassType*> out;
    for (auto type : types) {
        if (type->IsSubclassOf(Resource::GetStaticClass()) && !type->HasFlag(ClassFlags::Abstract) && type->HasFlag(ClassFlags::EditorVisible)) {
            out.push_back(type);
        }
    }
    return out;
}

void NodeContentBrowserPanel::SetCurrentDirectory(const std::filesystem::path &dir) {
    currentDirectory = dir;
    currentEntries.clear();

    std::set<std::filesystem::directory_entry> files_in_directory;
    std::set<std::filesystem::directory_entry> directories_in_directory;

    currentEntries.reserve(files_in_directory.size() + directories_in_directory.size());

    std::copy_if(std::filesystem::directory_iterator(currentDirectory), std::filesystem::directory_iterator(), std::inserter(directories_in_directory, directories_in_directory.begin()), [](const std::filesystem::directory_entry& entry)
    {
        return entry.is_directory();
    });

    std::copy_if(std::filesystem::directory_iterator(currentDirectory), std::filesystem::directory_iterator(), std::inserter(files_in_directory, files_in_directory.begin()), [](const std::filesystem::directory_entry& entry)
    {
        return !entry.is_directory() && (entry.path().extension() != ".meta");
    });

    for (auto& directory : directories_in_directory) {
        ContentBrowserEntry& e = currentEntries.emplace_back();
        e.Filename = directory.path().filename().string();
        e.Thumbnail = FolderTexture->GetInternalTextureView();
        e.DirectoryEntry = directory;
    }

    for (auto& file: files_in_directory) {
        ContentBrowserEntry& e = currentEntries.emplace_back();
        e.Filename = file.path().filename().string();
        e.DirectoryEntry = file;
        auto relPath = Filesystem::VirtualPath(file.path().string());


        if (ResourceManager::IsSourceFile(file.path())) {
            if (auto res = ResourceManager::Load(relPath)) {
                auto thumb = res->GetThumbnail();
                if (thumb) {
                    e.Thumbnail = thumb;
                }
            }
        }
    }
}
