module;
#pragma once

export module node_content_browser_panel;
import node;
import string;
import filesystem;

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
    virtual void Init() override;
    virtual void Ready() override;
    virtual void Update(float dt) override;
    void DrawGUI() override;

    float padding = 16.f;
    float itemSize = 64.f;
    
    std::filesystem::path currentDirectory;
    std::vector<std::filesystem::path> rootDirectories = {RESOURCE_DIR, ENGINE_RESOURCE_DIR};
    int currentRootIndex = 0;
};
