module;
#pragma once
#include "Renderer/MaterialHelpers.h"

export module node_viewport_ui;
import node_editor_camera_3d;
import node;
import renderer;
import object_ref;
import node_window;
import wgpu;
import view;
import crab_types;

namespace ImGuizmo
{
    struct matrix_t;
}

export class NodeViewportUI : public Node
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
