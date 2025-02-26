#pragma once
#include "Node.h"
#include "Renderer/Renderer.h"
#include "Utility/WeakRef.h"
#include "Nodes/NodeWindow.h"

class NodeViewportUI : public Node
{
public:
    NodeViewportUI() = default;

    explicit NodeViewportUI(const std::string& name)
        : Node(name)
    {
    }

    void Begin() override;
    void DrawGUI() override;
    void CopySurface();

    float GetAspectRatio() const;
    
    Vector2 windowSize = {200, 200};
    WeakRef<NodeWindow> ViewTarget;
    wgpu::Texture WindowViewTexture = nullptr;
    wgpu::Texture WindowRenderTexture = nullptr;
    wgpu::Texture WindowDepthTexture = nullptr;
    wgpu::TextureView ViewTextureView = nullptr;
    wgpu::TextureView RenderTextureView = nullptr;
    wgpu::TextureView DepthTextureView = nullptr;

    wgpu::TextureFormat depthFormat = wgpu::TextureFormat::Depth24Plus;
    
    using PostProcessBindGroupLayout = MaterialHelpers::BindGroupLayoutBuilder<MaterialHelpers::TextureEntry<0, FRAGMENT>>;
    
    void CreateRenderTexture(uint32_t width, uint32_t height);
    void CreateDepthTexture(uint32_t width, uint32_t height);
    void CreateRenderViewTexture(uint32_t width, uint32_t height);
};
