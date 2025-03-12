#pragma once
#include "NodeEditorCamera3D.h"
#include "Nodes/Node.h"
#include "Renderer/Renderer.h"
#include "Utility/WeakRef.h"
#include "Nodes/NodeWindow.h"

namespace ImGuizmo
{
    struct matrix_t;
}

class NodeViewportUI : public Node
{
public:
    NodeViewportUI() = default;

    void Ready() override;
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
    
    WeakRef<Node> selectedNode;
    void SetViewedNode(Node* node);

    using PostProcessBindGroupLayout = MaterialHelpers::BindGroupLayoutBuilder<MaterialHelpers::TextureEntry<0, FRAGMENT>>;
    
    void CreateRenderTexture(uint32_t width, uint32_t height);
    void CreateDepthTexture(uint32_t width, uint32_t height);
    void CreateRenderViewTexture(uint32_t width, uint32_t height);

    WeakRef<NodeEditorCamera3D> ActiveCamera;

    void EditTransform(const View& view, Matrix4& matrix);
};
