#include "NodeViewportUI.h"

#include "Nodes/NodeWindow.h"
#include "Resource/TextureResource.h"
//#include "ImGuizmo/ImGuizmo.h"
#include "imgui/imgui.h"
#include "ImGuizmo/ImGuizmo.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Application.h"

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
    CreateRenderViewTexture(windowSize.x, windowSize.y);
}

void NodeViewportUI::DrawGUI()
{
    Node::DrawGUI();

    ImGui::Begin("Viewport", 0);
    ViewTarget->SetSurfaceDrawEnabled(!ImGui::IsWindowDocked());
    
    ImVec2 currentWindowSize = ImGui::GetContentRegionAvail();
    if ((currentWindowSize.x != windowSize.x || currentWindowSize.y != windowSize.y) && currentWindowSize.x >= 1 && currentWindowSize.y >= 1)
    {
        windowSize.x = currentWindowSize.x;
        windowSize.y = currentWindowSize.y;
        CreateDepthTexture(windowSize.x, windowSize.y);
        CreateRenderTexture(windowSize.x, windowSize.y);
        CreateRenderViewTexture(windowSize.x, windowSize.y);
        //cam.ProjectionMatrix = glm::perspective(45 * PI / 180, GetAspectRatio(), 0.01f, 1000.0f);

    }

    View viewData;
    if (ActiveCamera)
    {
        viewData.Position = ActiveCamera->GetGlobalPosition();
        viewData.ViewMatrix = ActiveCamera->GetViewMatrix();
        viewData.ProjectionMatrix = glm::perspective(ActiveCamera->FOV * PI / 180, GetAspectRatio(), ActiveCamera->NearClippingPlane, ActiveCamera->FarClippingPlane);
    }
    else
    {
        if (ViewTarget)
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

    
    if (ViewTarget)
    {
        //auto view = ViewTarget->GetCurrentTextureView();
        //WGPUTextureView v = view;
        
        if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_None);

            auto move = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right, 0.f);
            if (ActiveCamera)
            {
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
                ///float yawAngle = -move.x * 0.01f;
                //float pitchAngle = -move.y * 0.01f;

                //orientation = glm::rotate(orientation, pitchAngle, right);
                //orientation = glm::rotate(orientation, yawAngle, up);
                //orientation = glm::normalize(rotation * orientation);
                
                //ActiveCamera->SetGlobalOrientation(orientation);
                
                ImGui::ResetMouseDragDelta(ImGuiMouseButton_Right);
            }
        }
        if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
        {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
        }
        
        
        ViewTarget->GetRenderer().RenderNodeTree(ViewTarget.Get(), viewData, RenderTextureView, DepthTextureView);
        
        CopySurface();
        ImGui::Image((uint64_t)(WGPUTextureView)ViewTextureView, ImGui::GetContentRegionAvail());
    }



    
    if (selectedNode.IsValid())
    {
        if (auto node3d = dynamic_cast<Node3D*>(selectedNode.Get()))
        {
            auto t = selectedNode->GetTransform();
            auto mat = t.ModelMatrix;
    
            static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::OPERATION::TRANSLATE);
            static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
            if (ImGui::IsKeyPressed(ImGuiKey_W))
                mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
            if (ImGui::IsKeyPressed(ImGuiKey_E))
                mCurrentGizmoOperation = ImGuizmo::ROTATE;
            if (ImGui::IsKeyPressed(ImGuiKey_R)) // r Key
                mCurrentGizmoOperation = ImGuizmo::SCALE;
            if (ImGui::IsKeyPressed(ImGuiKey_Q)) // r Key
                mCurrentGizmoOperation = ImGuizmo::UNIVERSAL;

            ImGuizmo::SetDrawlist();
            ImGuiIO& io = ImGui::GetIO();
            auto pos = ImGui::GetWindowPos();
            auto size = ImGui::GetWindowContentRegionMax();
            ImGuizmo::SetOrthographic(false);
            ImGuizmo::SetRect(pos.x, pos.y, size.x, size.y);


            
            //auto projMat = glm::perspective(45.0f * PI / 180.f, GetAspectRatio(), 0.01f, 1000.0f);
            
            if (ImGuizmo::Manipulate(glm::value_ptr(viewData.ViewMatrix), glm::value_ptr(viewData.ProjectionMatrix), mCurrentGizmoOperation, mCurrentGizmoMode, glm::value_ptr(mat), NULL, NULL))
            {
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
            
        }
    }
    
    ImGui::End();

}

void NodeViewportUI::CopySurface()
{
    wgpu::CommandEncoderDescriptor encoderDesc = {};
    encoderDesc.label = "Window Initial Copy";
    wgpu::CommandEncoder encoder = Application::Get().GetDevice().createCommandEncoder(encoderDesc);
    
    wgpu::ImageCopyTexture src;
    src.texture = WindowRenderTexture;

    wgpu::ImageCopyTexture dest;
    dest.texture = WindowViewTexture;
    
    encoder.copyTextureToTexture(src, dest, wgpu::Extent3D(WindowRenderTexture.getWidth(), WindowRenderTexture.getHeight(), 1));
    
    auto c = encoder.finish();
    ViewTarget->GetRenderer().AddCommand(c);
    
    encoder.release();
    
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
    if (WindowRenderTexture) WindowRenderTexture.release();
    WindowRenderTexture = nullptr;
    if (RenderTextureView) RenderTextureView.release();
    RenderTextureView = nullptr;
    
    wgpu::TextureDescriptor desc;
    desc.size = {width, height, 1};
    desc.mipLevelCount = 1;
    desc.sampleCount = 1;
    desc.dimension = wgpu::TextureDimension::_2D;
    desc.format = wgpu::TextureFormat::BGRA8UnormSrgb;
    desc.usage = wgpu::TextureUsage::RenderAttachment | wgpu::TextureUsage::CopySrc;
    
    WindowRenderTexture = Application::Get().GetDevice().createTexture(desc);
    
    WGPUTextureViewDescriptor vd;
    vd.label = "Viewport View";
    vd.format = wgpu::TextureFormat::BGRA8UnormSrgb;
    vd.dimension = wgpu::TextureViewDimension::_2D;
    vd.baseMipLevel = 0;
    vd.mipLevelCount = 1;
    vd.baseArrayLayer = 0;
    vd.arrayLayerCount = 1;
    vd.aspect = wgpu::TextureAspect::All;
    RenderTextureView = WindowRenderTexture.createView(vd);
}

void NodeViewportUI::CreateRenderViewTexture(uint32_t width, uint32_t height)
{
    if (WindowViewTexture) WindowViewTexture.release();
    WindowViewTexture = nullptr;
    if (ViewTextureView) ViewTextureView.release();
    ViewTextureView = nullptr;
    
    wgpu::TextureDescriptor desc;
    desc.size = {width, height, 1};
    desc.mipLevelCount = 1;
    desc.sampleCount = 1;
    desc.dimension = wgpu::TextureDimension::_2D;
    desc.format = wgpu::TextureFormat::BGRA8Unorm;
    desc.usage = wgpu::TextureUsage::TextureBinding | wgpu::TextureUsage::CopyDst;
    
    WindowViewTexture = Application::Get().GetDevice().createTexture(desc);
    
    WGPUTextureViewDescriptor vd;
    vd.label = "Viewport View";
    vd.format = wgpu::TextureFormat::BGRA8Unorm;
    vd.dimension = wgpu::TextureViewDimension::_2D;
    vd.baseMipLevel = 0;
    vd.mipLevelCount = 1;
    vd.baseArrayLayer = 0;
    vd.arrayLayerCount = 1;
    vd.aspect = wgpu::TextureAspect::All;
    ViewTextureView = WindowViewTexture.createView(vd);
}

void NodeViewportUI::CreateDepthTexture(uint32_t width, uint32_t height)
{
    if (WindowDepthTexture) WindowDepthTexture.release();
    WindowDepthTexture = nullptr;
    if (DepthTextureView) DepthTextureView.release();
    DepthTextureView = nullptr;
    
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
    vd.label = "Viewport View";
    vd.format = depthFormat;
    vd.dimension = wgpu::TextureViewDimension::_2D;
    vd.baseMipLevel = 0;
    vd.mipLevelCount = 1;
    vd.baseArrayLayer = 0;
    vd.arrayLayerCount = 1;
    vd.aspect = wgpu::TextureAspect::All;
    DepthTextureView = WindowDepthTexture.createView(vd);


}
