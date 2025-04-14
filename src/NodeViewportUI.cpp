module;
#include "imgui.h"
#include "ImGuizmo/ImGuizmo.h"

module node_viewport_ui;
import Engine.GLM;
import Engine.Node;
import Engine.Application;
import Engine.Resource.Texture;
import Engine.Node.Window;
import Engine.Math;
import Engine.Compute.ClearTexture;


void NodeViewportUI::Ready()
{
    Node::Ready();

    if (auto window = GetAncestorOfType<NodeWindow>())
    {
        ViewTarget = window;
        
    }

    //cam.ProjectionMatrix = glm::perspective(45 * PI / 180, GetAspectRatio(), 0.01f, 1000.0f);
   // cam.ViewMatrix = glm::lookAt(glm::vec3(4.0f, 0.f, 0.0f), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
    //cam.Position = glm::vec3(4.0f, 0.f, 0.0f);
    
    CreateDepthTexture(windowSize.x, windowSize.y);
    CreateRenderTexture(windowSize.x, windowSize.y);
    CreateViewTexture(windowSize.x, windowSize.y);
    CreateIDPassTextures(windowSize.x, windowSize.y);
}

void NodeViewportUI::DrawGUI()
{
    Node::DrawGUI();
    ImGui::Begin("Viewport", 0);

    ImVec2 currentWindowSize = ImGui::GetContentRegionAvail();
    if ((currentWindowSize.x != windowSize.x || currentWindowSize.y != windowSize.y) && currentWindowSize.x >= 1 && currentWindowSize.y >= 1 && resizeCooldown <= 0.f)
    {
        windowSize.x = currentWindowSize.x;
        windowSize.y = currentWindowSize.y;
        CreateDepthTexture(windowSize.x, windowSize.y);
        CreateRenderTexture(windowSize.x, windowSize.y);
        CreateViewTexture(windowSize.x, windowSize.y);
        CreateIDPassTextures(windowSize.x, windowSize.y);

        std::cout << "Viewport Resize" << std::endl;
        resizeCooldown = 0.25f;
    }

    clearTexturePass.Execute(PickingPassTexture);

    View viewData;
    if (ActiveCamera.IsValid())
    {
        viewData.Position = ActiveCamera->GetGlobalPosition();
        viewData.ViewMatrix = ActiveCamera->GetViewMatrix();
        viewData.ProjectionMatrix = glm::perspective(ActiveCamera->FOV * PI / 180, GetAspectRatio(), ActiveCamera->NearClippingPlane, ActiveCamera->FarClippingPlane);
    }
    else
    {
        if (ViewTarget.IsValid())
        {
            if (auto editorCam = dynamic_cast<NodeEditorCamera3D*>(ViewTarget->ActiveCamera.Get()))
            {
                ActiveCamera = editorCam;
            }
        }
        else
        {
            viewData.ProjectionMatrix = glm::perspective(45.0f * PI / 180.f, GetAspectRatio(), 0.01f, 1000.f);
        }
    }
    
    if (ImGui::IsKeyPressed(ImGuiKey_Escape))
    {
        selectedNode = nullptr;
        OnNodeSelectedInViewport.invoke(nullptr);
    }

    ImVec2 imageSize = ImGui::GetContentRegionAvail();
    ImVec2 uv;
    bool bIsMouseOverViewport = false;


    if (ViewTarget.IsValid())
    {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right && ImGui::IsWindowHovered())) {
            InitialClickPos = {ImGui::GetMousePos().x, ImGui::GetMousePos().y};
        }

        if (ImGui::IsMouseDown(ImGuiMouseButton_Right) && ImGui::IsWindowHovered())
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_None);
            Vector2 CurrentPos = {ImGui::GetMousePos().x, ImGui::GetMousePos().y};
            Vector2 Drag = CurrentPos - InitialClickPos;
            //ImVec2 move = {Drag.x, Drag.y};
            //ImGui::GetIO().WantSetMousePos = true;
           // ImGui::GetIO().MousePos = {InitialClickPos.x, InitialClickPos.y};
            auto move = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right, 0.f);
            ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);

            if (ActiveCamera)
            {
                bIsControllingCamera = true;
                auto orientation = ActiveCamera->GetGlobalOrientation();
                auto up = ActiveCamera->GetUpVector();
                auto right = ActiveCamera->GetRightVector();
                
                ActiveCamera->HandleMouseMovement({move.x, move.y});

                Vector3 movement{0,0,0};
                if (ImGui::IsKeyDown(ImGuiKey_W))
                {
                    movement += Vector3{1, 0, 0};
                }
                if (ImGui::IsKeyDown(ImGuiKey_S))
                {
                    movement -= Vector3{1, 0, 0};
                }
                if (ImGui::IsKeyDown(ImGuiKey_A))
                {
                    movement -= Vector3{0, 1, 0};
                }
                if (ImGui::IsKeyDown(ImGuiKey_D))
                {
                    movement += Vector3{0, 1, 0};
                }
                if (ImGui::IsKeyDown(ImGuiKey_E))
                {
                    movement += Vector3{0, 0, 1};
                }
                if (ImGui::IsKeyDown(ImGuiKey_Q))
                {
                    movement -= Vector3{0, 0, 1};
                }
                ActiveCamera->HandleKeyboardMovement(movement);
            } else bIsControllingCamera = false;
        }
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
            bIsControllingCamera = false;
        }
        
        // Normal render pass
        // Normal render pass
        RenderedNodes = ViewTarget->GetRenderer().RenderNodeTree(ViewTarget.Get(), viewData, *RenderTextureView, *DepthTextureView, PickingPassTexture);
        
        CopySurface();


        ImGui::Image((uint64_t)(WGPUTextureView)*ViewTextureView, imageSize);
        if (ImGui::IsItemHovered())
        {
            bIsMouseOverViewport = true;

            ImVec2 mouse_pos = ImGui::GetMousePos();
            ImVec2 image_min = ImGui::GetItemRectMin();
            ImVec2 image_max = ImGui::GetItemRectMax();
        
            // 3. Calculate UV coordinates with proper scaling
            uv = ImVec2(
                (mouse_pos.x - image_min.x) / (image_max.x - image_min.x),
                (mouse_pos.y - image_min.y) / (image_max.y - image_min.y)
            );
        }
        
    }
    
    
    if (selectedNode.IsValid())
    {
        if (auto node3d = dynamic_cast<Node3D*>(selectedNode.Get()))
        {
            auto t = selectedNode->GetTransform();
            auto mat = t.ModelMatrix;
    
            static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::OPERATION::TRANSLATE);
            static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
            if (ImGui::IsKeyChordPressed(ImGuiMod_Shift | ImGuiKey_Tab))
            {
                mCurrentGizmoMode = mCurrentGizmoMode == ImGuizmo::LOCAL ? ImGuizmo::WORLD : ImGuizmo::LOCAL;
            }
            
            if (!bIsControllingCamera && ImGui::IsWindowFocused())
            {
                if (ImGui::IsKeyPressed(ImGuiKey_W, false))
                    mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
                if (ImGui::IsKeyPressed(ImGuiKey_E, false))
                    mCurrentGizmoOperation = ImGuizmo::ROTATE;
                if (ImGui::IsKeyPressed(ImGuiKey_R, false)) // r Key
                    mCurrentGizmoOperation = ImGuizmo::SCALE;
                if (ImGui::IsKeyPressed(ImGuiKey_Q, false)) // r Key
                    mCurrentGizmoOperation = ImGuizmo::UNIVERSAL;
            }
            
            ImGuizmo::SetDrawlist();
            auto pos = ImGui::GetWindowPos();
            auto size = ImGui::GetWindowContentRegionMax();
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);


            
            //auto projMat = glm::perspective(45.0f * PI / 180.f, GetAspectRatio(), 0.01f, 1000.0f);
            
            if (ImGuizmo::Manipulate(glm::value_ptr(viewData.ViewMatrix), glm::value_ptr(viewData.ProjectionMatrix), mCurrentGizmoOperation, mCurrentGizmoMode, glm::value_ptr(mat), nullptr, nullptr))
            {
                if (!bDragActive && ImGui::IsKeyDown(ImGuiKey_LeftAlt))
                {
                    selectedNode->GetParent()->AddChild(node3d->Duplicate());
                }
                bDragActive = true;
                
                glm::vec3 scale;
                glm::quat rotation;
                glm::vec3 translation;

                MathUtils::DecomposeTransform(mat, translation, rotation, scale);

                glm::quat deltaRotation = rotation - node3d->GetGlobalOrientation();
                Vector3 deltaScale = scale - node3d->GetGlobalScale();
                
                node3d->SetGlobalPosition(translation);
                node3d->SetGlobalOrientation(node3d->GetGlobalOrientation() + deltaRotation);
                node3d->SetGlobalScale(node3d->GetGlobalScale() + deltaScale);
            }
            else
            {
                if (bDragActive && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
                    bDragActive = false;
            }
            
        }
    }


        if (bIsMouseOverViewport && ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !bIsControllingCamera && !ImGuizmo::IsViewManipulateHovered() && !ImGuizmo::IsUsingAny() && !bDragActive)
        {
            // 4. Convert to pixel coordinates (account for texture aspect ratio)
            int click_x = static_cast<int>(uv.x * imageSize.x);
            int click_y = static_cast<int>(uv.y * imageSize.y);
        
            // Handle mouse clicks
            wgpu::BufferDescriptor desc;
            desc.size = sizeof(uint32_t);
            desc.usage = wgpu::BufferUsage::CopyDst | wgpu::BufferUsage::MapRead;

            currentReadbackBuffer = Application::Get().GetDevice().createBuffer(desc);

            // Copy pixel data
            wgpu::TexelCopyTextureInfo source;
                source.texture = *PickingPassTexture->GetInternalTexture();
                source.mipLevel = 0;
                source.origin = WGPUOrigin3D(click_x, click_y, 0);

            wgpu::TexelCopyBufferInfo destination;
                destination.buffer = *currentReadbackBuffer;
                WGPUTexelCopyBufferLayout l;
                l.rowsPerImage = PickingPassTexture->GetInternalTexture()->getHeight();
                l.bytesPerRow = 256;
                l.offset = 0;
                destination.layout = l;
            

            wgpu::Extent3D copy_size = {1, 1, 1};
            wgpu::CommandEncoderDescriptor cmdDesc;
            wgpu::raii::CommandEncoder commandEncoder = Application::Get().GetDevice().createCommandEncoder(cmdDesc);
            commandEncoder->copyTextureToBuffer(source, destination, copy_size);
            Application::Get().GetQueue().submit(commandEncoder->finish());

            // 3. Map asynchronously with proper callback
            auto on_map = [](WGPUMapAsyncStatus status, WGPUStringView message, void* userdata1 /* this */, void* userdata2) {
                if (status == wgpu::MapAsyncStatus::Success) {
                    NodeViewportUI* viewport = static_cast<NodeViewportUI*>(userdata1);
                    const uint32_t* data = static_cast<const uint32_t*>(

                        viewport->currentReadbackBuffer->getMappedRange(0, sizeof(uint32_t))
                    );
            
                    // Copy data immediately before unmapping
                    uint32_t result = *data;
                    viewport->currentReadbackBuffer->unmap();
                    viewport->OnPixelValueClicked(result);
                } else {
                    // Handle error
                    std::cerr << "Buffer mapping failed: " << status << "\n";
                }
            };
            WGPUBufferMapCallbackInfo callback;
            callback.userdata1 = this;
            callback.callback = on_map;
            currentReadbackBuffer->mapAsync(wgpu::MapMode::Read, 0, sizeof(uint32_t), callback);
        }

    ImGui::End();

}

void NodeViewportUI::CopySurface() {
    wgpu::CommandEncoderDescriptor encoderDesc = {};
    encoderDesc.label = {"Window Initial Copy", wgpu::STRLEN};
    wgpu::raii::CommandEncoder encoder = Application::Get().GetDevice().createCommandEncoder(encoderDesc);

    wgpu::TexelCopyTextureInfo src;
    src.texture = *WindowRenderTexture;

    wgpu::TexelCopyTextureInfo dest;
    dest.texture = *WindowViewTexture;

    encoder->copyTextureToTexture(src, dest, wgpu::Extent3D(WindowRenderTexture->getWidth(), WindowRenderTexture->getHeight(), 1));

    wgpu::raii::CommandBuffer c = encoder->finish();
    ViewTarget->GetRenderer().AddCommand(c);
}

void NodeViewportUI::CreateViewTexture(uint32_t width, uint32_t height)
{
    wgpu::TextureDescriptor desc;
    desc.size = {width, height, 1};
    desc.mipLevelCount = 1;
    desc.sampleCount = 1;
    desc.dimension = wgpu::TextureDimension::_2D;
    desc.format = wgpu::TextureFormat::BGRA8Unorm;
    desc.usage = wgpu::TextureUsage::TextureBinding | wgpu::TextureUsage::CopyDst;
    WindowViewTexture = Application::Get().GetDevice().createTexture(desc);


    WGPUTextureViewDescriptor vd;
    vd.label = {"Viewport View", wgpu::STRLEN};
    vd.format = wgpu::TextureFormat::BGRA8Unorm;
    vd.dimension = wgpu::TextureViewDimension::_2D;
    vd.baseMipLevel = 0;
    vd.mipLevelCount = 1;
    vd.baseArrayLayer = 0;
    vd.arrayLayerCount = 1;
    vd.aspect = wgpu::TextureAspect::All;
    ViewTextureView = WindowViewTexture->createView(vd);
}

void NodeViewportUI::CreateIDPassTextures(uint32_t width, uint32_t height)
{
    if (!PickingPassTexture)
    {
        PickingPassTexture = std::make_shared<RuntimeTextureResource>();
    }
    
    wgpu::TextureDescriptor IDPassDesc = wgpu::Default;
    IDPassDesc.size.width = width;
    IDPassDesc.size.height = height;
    IDPassDesc.size.depthOrArrayLayers = 1;
    IDPassDesc.usage = wgpu::TextureUsage::CopySrc | wgpu::TextureUsage::StorageBinding;
    IDPassDesc.format = wgpu::TextureFormat::R32Uint;
    IDPassDesc.mipLevelCount = 1;
    IDPassDesc.sampleCount = 1;
    IDPassDesc.dimension = wgpu::TextureDimension::_2D;

    PickingPassTexture->CreateBlankTexture(IDPassDesc);
}

void NodeViewportUI::OnPixelValueClicked(uint32_t value) {
    if (value > 0)
    {
        Node* n = RenderedNodes.at(value);
        if (n)
        {
            selectedNode = n;
            OnNodeSelectedInViewport.invoke(n);
        }
        else
        {
            selectedNode = nullptr;
            OnNodeSelectedInViewport.invoke(nullptr);
        }
    }
    else
    {
        selectedNode = nullptr;
        OnNodeSelectedInViewport.invoke(nullptr);
    }
}


void NodeViewportUI::Update(float dt) {
    Node::Update(dt);
    resizeCooldown -= dt;
    resizeCooldown = std::max(0.f, resizeCooldown);
}

float NodeViewportUI::GetAspectRatio() const
{
    return windowSize.x / windowSize.y;
}

void NodeViewportUI::SetViewedNode(Node* node)
{
    selectedNode = node;
}

void NodeViewportUI::CreateRenderTexture(uint32_t width, uint32_t height)
{
    wgpu::TextureDescriptor desc;
    desc.size = {width, height, 1};
    desc.mipLevelCount = 1;
    desc.sampleCount = 1;
    desc.dimension = wgpu::TextureDimension::_2D;
    desc.format = wgpu::TextureFormat::BGRA8UnormSrgb;
    desc.usage = wgpu::TextureUsage::RenderAttachment | wgpu::TextureUsage::CopySrc;
    
    WindowRenderTexture = Application::Get().GetDevice().createTexture(desc);
    
    WGPUTextureViewDescriptor vd;
    vd.label = {"Viewport View", wgpu::STRLEN};
    vd.format = wgpu::TextureFormat::BGRA8UnormSrgb;
    vd.dimension = wgpu::TextureViewDimension::_2D;
    vd.baseMipLevel = 0;
    vd.mipLevelCount = 1;
    vd.baseArrayLayer = 0;
    vd.arrayLayerCount = 1;
    vd.aspect = wgpu::TextureAspect::All;
    vd.usage = desc.usage;
    RenderTextureView = WindowRenderTexture->createView(vd);
}

void NodeViewportUI::CreateDepthTexture(uint32_t width, uint32_t height)
{
    wgpu::TextureDescriptor desc;
    desc.size = {width, height, 1};
    desc.mipLevelCount = 1;
    desc.sampleCount = 1;
    desc.dimension = wgpu::TextureDimension::_2D;
    desc.format = depthFormat;
    desc.usage = wgpu::TextureUsage::RenderAttachment;
    desc.viewFormatCount = 1;
    desc.viewFormats = (WGPUTextureFormat*)&depthFormat;
    WindowDepthTexture = Application::Get().GetDevice().createTexture(desc);

    WGPUTextureViewDescriptor vd;
    vd.label = {"Viewport View", wgpu::STRLEN};
    vd.format = depthFormat;
    vd.dimension = wgpu::TextureViewDimension::_2D;
    vd.baseMipLevel = 0;
    vd.mipLevelCount = 1;
    vd.baseArrayLayer = 0;
    vd.arrayLayerCount = 1;
    vd.aspect = wgpu::TextureAspect::All;
    vd.usage = wgpu::TextureUsage::RenderAttachment;
    DepthTextureView = WindowDepthTexture->createView(vd);
}
