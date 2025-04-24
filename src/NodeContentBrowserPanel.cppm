module;
#pragma once
#include "ReflectionMacros.h"

export module node_content_browser_panel;
import Engine.Node;
import std;
import Engine.WGPU;

export class TextureResource;


export struct ResourcePathDragDropData
{
    ResourcePathDragDropData(const std::string& path)
    {
        path_raw = new char[path.size() + 1];
        path.copy(path_raw, path.size());
        path_raw[path.size()] = '\0';
    }
    
    std::string GetPath()
    {
        return {path_raw};
    }
    
    char* path_raw;
};

export class NodeContentBrowserPanel : public Node
{
public:
    CRAB_CLASS(NodeContentBrowserPanel, Node)
    virtual void Init() override;
    virtual void Ready() override;
    virtual void Update(float dt) override;
    void DrawGUI() override;

    enum EReply {
        Success,
        ReconstructingAssetPanel
    };

    struct ContentBrowserEntry {
        std::string Filename{};
        std::filesystem::directory_entry DirectoryEntry{};
        std::optional<wgpu::raii::TextureView> Thumbnail{};
    };

    EReply DrawAssetWidget(const ContentBrowserEntry& entry);
    void DrawResourceCreationMenu();

    std::vector<const ClassType*> GetAvailableResourceTypes();

    float padding = 16.f;
    float itemSize = 64.f;

    void SetCurrentDirectory(const std::filesystem::path& dir);
    void RefreshBrowser();

    struct RootDirectory {
        std::string AbsolutePath;
        std::string Alias;
    };

    std::filesystem::path currentDirectory;
    std::vector<ContentBrowserEntry> currentEntries;
    std::vector<RootDirectory> rootDirectories = {{RESOURCE_DIR, "/res"}, {ENGINE_RESOURCE_DIR, "/engine"}};
    int currentRootIndex = 0;

    std::shared_ptr<TextureResource> FolderTexture;
};

void NodeContentBrowserPanel::RefreshBrowser() {
    SetCurrentDirectory(currentDirectory);
}
