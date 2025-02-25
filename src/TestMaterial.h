#pragma once
#include "Gfx/Vertex.h"
#include "Renderer/DynamicUniformBuffer.h"
#include "Renderer/Material.h"
#include "Renderer/UniformBuffer.h"

class TestMaterial : public Material
{
public:
    /*
    BEGIN_LAYOUT(TestMaterialUniforms)
        UNIFORM_BUFFER(0, wgpu::ShaderStage::Vertex)
    END_LAYOUT()

    BEGIN_LAYOUT(PerObjectUniforms)
        UNIFORM_BUFFER(0, wgpu::ShaderStage::Vertex)
    END_LAYOUT()
    */
    using PerObjectUniformsLayout = MaterialHelpers::BindGroupLayoutBuilder<
        MaterialHelpers::UniformBufferEntry<0, wgpu::ShaderStage::Vertex>>;
    
    using TestMaterialUniformsLayout = MaterialHelpers::BindGroupLayoutBuilder<
        MaterialHelpers::UniformBufferEntry<0, wgpu::ShaderStage::Vertex | wgpu::ShaderStage::Fragment>>;

    TestMaterial(const wgpu::Device& device, const std::filesystem::path& shaderPath, const MaterialSettings& settings = MaterialSettings())
        : Material(device, shaderPath, settings),
          perObjectBuffer(device),
          uniformBuffer(device)
    {
    }

    std::vector<wgpu::BindGroupLayout> CreateMaterialBindGroupLayouts() override;
    std::vector<MaterialBindGroup> CreateMaterialBindGroups() override;
    void CreateVertexBufferLayouts(std::vector<Vertex::VertexBufferLayout>& Layouts) override;
    void UpdateUniforms() override;

public:
    UniformBuffer<mat4x4> perObjectBuffer;
    UniformBuffer<Application::MyUniforms> uniformBuffer;
};
