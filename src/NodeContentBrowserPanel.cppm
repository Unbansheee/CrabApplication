module;
#pragma once
#include "ReflectionMacros.h"

export module node_content_browser_panel;
import Engine.Node;
import std;


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

    void DrawAssetWidget(const std::filesystem::directory_entry& entry);

    float padding = 16.f;
    float itemSize = 64.f;
    
    std::filesystem::path currentDirectory;
    std::vector<std::filesystem::path> rootDirectories = {RESOURCE_DIR, ENGINE_RESOURCE_DIR};
    int currentRootIndex = 0;
};
