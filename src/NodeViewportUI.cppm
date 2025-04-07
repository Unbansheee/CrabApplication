module;
#pragma once
#include "ReflectionMacros.h"
#include "Renderer/MaterialHelpers.h"

export module node_viewport_ui;
import node_editor_camera_3d;
import Engine.Node;
import Engine.GFX.Renderer;
import Engine.Object.Ref;
import Engine.Node.Window;
import Engine.WGPU;
import Engine.GFX.View;
import Engine.Types;
import Engine.Resource.RuntimeTexture;
import Engine.GFX.IDPassMaterial;
import Engine.GFX.IDPassRenderer;
import Engine.WGPU;
import Engine.Compute.ClearTexture;

namespace ImGuizmo
{
    struct matrix_t;
}

export class NodeViewportUI : public Node
{
public:
    public:
    NodeViewportUI() = default;

    void Ready() override;
    void DrawGUI() override;
    void Update(float dt) override;
    void CopySurface();
    float GetAspectRatio() const;

    float resizeCooldown = 0.0f;
    
    Vector2 windowSize = {200, 200};
    ObjectRef<NodeWindow> ViewTarget;
    wgpu::Texture WindowRenderTexture = nullptr;
    wgpu::Texture WindowDepthTexture = nullptr;
    wgpu::TextureView RenderTextureView = nullptr;
    wgpu::TextureView DepthTextureView = nullptr;
    wgpu::TextureView ViewTextureView = nullptr;
    wgpu::Texture WindowViewTexture = nullptr;

    rocket::signal<void(Node*)> OnNodeSelectedInViewport;
    
    std::shared_ptr<RuntimeTextureResource> PickingPassTexture;
    ComputeClearTexture<WGPUTextureFormat_R32Uint, WGPUStorageTextureAccess_WriteOnly> clearTexturePass;
    
    wgpu::TextureFormat depthFormat = wgpu::TextureFormat::Depth24Plus;
    
    ObjectRef<Node> selectedNode;
    void SetViewedNode(Node* node);
    std::unique_ptr<wgpu::BufferMapCallback> bufferCallbackFunc;
    wgpu::Buffer currentReadbackBuffer = nullptr;

    bool bDragActive = false;
    bool bIsControllingCamera = false;
    
    using PostProcessBindGroupLayout = MaterialHelpers::BindGroupLayoutBuilder<MaterialHelpers::TextureEntry<0, FRAGMENT>>;
    
    void CreateRenderTexture(uint32_t width, uint32_t height);
    void CreateDepthTexture(uint32_t width, uint32_t height);
    void CreateViewTexture(uint32_t width, uint32_t height);
    void CreateIDPassTextures(uint32_t width, uint32_t height);

    ObjectRef<NodeEditorCamera3D> ActiveCamera;

    void EditTransform(const View& view, Matrix4& matrix);
};
