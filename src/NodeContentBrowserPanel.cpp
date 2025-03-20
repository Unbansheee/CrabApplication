module;
#include <filesystem>
#include <set>
#include "imgui.h"
#include "imgui_internal.h"

module node_content_browser_panel;

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
    currentDirectory = ENGINE_RESOURCE_DIR;
}

void NodeContentBrowserPanel::Ready()
{
    Node::Ready();
}

void NodeContentBrowserPanel::Update(float dt)
{
    Node::Update(dt);
}



void NodeContentBrowserPanel::DrawGUI()
{
    Node::DrawGUI();

    ImGui::Begin("Content Browser");

    if (ImGui::Button("<-"))
    {
        if (std::ranges::find(rootDirectories, currentDirectory) == rootDirectories.end() )
        {
            currentDirectory = currentDirectory.parent_path();
        }
    }
    ImGui::SameLine();
    
    ImGui::Text(currentDirectory.string().c_str());
    ImGui::Separator();
    
    //ImGui::Columns(2);
    ImGui::BeginChild("SourcesPanel", {}, ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AlwaysAutoResize);
    int _rootSelectIndex = 0;
    for (auto& root : rootDirectories)
    {
        if (ImGui::Button((root.parent_path().filename().string() + "/" + root.filename().string()).c_str()))
        {
            currentDirectory = root;
            currentRootIndex = _rootSelectIndex;
        }
        _rootSelectIndex++;
    }
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);

    ImGui::SameLine();
    //ImGui::NextColumn();
    ImGui::BeginChild("BrowserPanel");
    float panelWidth = ImGui::GetContentRegionAvail().x;
    float cellSize = padding + itemSize;
    int columnCount = (int)(panelWidth / cellSize);
    columnCount = std::max(columnCount, 1);

    
    std::set<std::filesystem::directory_entry> files_in_directory;
    std::set<std::filesystem::directory_entry> directories_in_directory;

    std::copy_if(std::filesystem::directory_iterator(currentDirectory), std::filesystem::directory_iterator(), std::inserter(directories_in_directory, directories_in_directory.begin()), [](const std::filesystem::directory_entry& entry)
    {
        return entry.is_directory();
    });

    std::copy_if(std::filesystem::directory_iterator(currentDirectory), std::filesystem::directory_iterator(), std::inserter(files_in_directory, files_in_directory.begin()), [](const std::filesystem::directory_entry& entry)
    {
        return !entry.is_directory() && (entry.path().extension() != ".importSettings");
    });

    ImGui::Columns(columnCount, 0, false);

    for (auto& i : directories_in_directory)
    {
        DrawAssetWidget(i);
        ImGui::NextColumn();
    }
    
    for (auto& i : files_in_directory)
    {
        DrawAssetWidget(i);
        ImGui::NextColumn();
    }
    
    ImGui::Columns(1);
    ImGui::EndChild();
    ImGui::Columns(1);

    ImGui::End();
}

void NodeContentBrowserPanel::DrawAssetWidget(const std::filesystem::directory_entry& entry)
{
    std::string path = entry.path().filename().string();
    if (entry.is_directory())
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.7f, 1.0f));
    }
        
    ImGui::Button(("##" + path).c_str(), {itemSize, itemSize});
    if (ImGui::BeginDragDropSource())
    {
        ResourcePathDragDropData data(entry.path().string());
        ImGui::SetDragDropPayload("RESOURCE_PATH", &data, sizeof(ResourcePathDragDropData));
        ImGui::EndDragDropSource();
    }
    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
    {
        if (entry.is_directory())
        {
            currentDirectory /= entry.path().filename();
        }
    }
    ImGui::TextWrapped(path.c_str());
    if (entry.is_directory())
    {
        ImGui::PopStyleColor();
    }
}
