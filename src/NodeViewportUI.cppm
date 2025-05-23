﻿module;
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
import Engine.WGPU;
import Engine.Compute.ClearTexture;

namespace ImGuizmo
{
    struct matrix_t;
}

export class NodeViewportUI : public Node
{
    CRAB_CLASS(NodeViewportUI, Node)
    BEGIN_PROPERTIES
    END_PROPERTIES

public:
    rocket::signal<void(Node*)> OnNodeSelectedInViewport;
    ObjectRef<Node> selectedNode;

private:
    void Ready() override;
    void DrawGUI() override;
    void Update(float dt) override;

    void CopySurface();
    float GetAspectRatio() const;
    void CreateRenderTexture(uint32_t width, uint32_t height);
    void CreateDepthTexture(uint32_t width, uint32_t height);
    void CreateViewTexture(uint32_t width, uint32_t height);
    void CreateIDPassTextures(uint32_t width, uint32_t height);
    void OnPixelValueClicked(uint32_t value);
    void SetViewedNode(Node* node);
    InputResult HandleInput(const InputEvent &event) override;
    void EditTransform(const View& view, Matrix4& matrix);


    wgpu::raii::Texture WindowRenderTexture{};
    wgpu::raii::Texture WindowDepthTexture{};
    wgpu::raii::TextureView RenderTextureView{};
    wgpu::raii::TextureView DepthTextureView{};
    wgpu::raii::TextureView ViewTextureView{};
    wgpu::raii::Texture WindowViewTexture{};

    float resizeCooldown = 0.0f;
    Vector2 windowSize = {200, 200};
    ObjectRef<NodeWindow> ViewTarget;
    std::shared_ptr<RuntimeTextureResource> PickingPassTexture;
    ComputeClearTexture<WGPUTextureFormat_R32Uint, WGPUStorageTextureAccess_WriteOnly> clearTexturePass;
    wgpu::TextureFormat depthFormat = wgpu::TextureFormat::Depth24Plus;
    
    std::unique_ptr<wgpu::BufferMapCallback> bufferCallbackFunc;
    wgpu::raii::Buffer currentReadbackBuffer = nullptr;

    bool bDragActive = false;
    bool bIsControllingCamera = false;
    
    std::vector<Node*> RenderedNodes;
    ObjectRef<NodeEditorCamera3D> ActiveCamera;

    Vector2 InitialClickPos;
    std::queue<Vector2> mouseMoveBuffer;
};

