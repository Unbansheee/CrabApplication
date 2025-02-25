#include "TestMaterial.h"

#include "Gfx/Vertex.h"

std::vector<wgpu::BindGroupLayout> TestMaterial::CreateMaterialBindGroupLayouts()
{
    return {
        PerObjectUniformsLayout::Create(m_device),
        TestMaterialUniformsLayout::Create(m_device)
    };   
}

std::vector<Material::MaterialBindGroup> TestMaterial::CreateMaterialBindGroups()
{
    MaterialHelpers::BindGroupCreator<PerObjectUniformsLayout> perObjectBindsCreator(m_device);
    auto perObjectGroup = perObjectBindsCreator
        .Set<0, WGPUBuffer>(perObjectBuffer.GetBuffer())
        .Build();
    
    MaterialHelpers::BindGroupCreator<TestMaterialUniformsLayout> uniformsBindsCreator(m_device);
    auto uniformGroup = uniformsBindsCreator
        .Set<0, WGPUBuffer>(uniformBuffer.GetBuffer())
        .Build();

    return {{0, perObjectGroup}, {1, uniformGroup}};
}

void TestMaterial::CreateVertexBufferLayouts(std::vector<Vertex::VertexBufferLayout>& Layouts)
{
    auto& a = Layouts.emplace_back();
    Vertex::CreateVertexBufferLayout<MeshVertex>(a);
}

void TestMaterial::UpdateUniforms()
{
}
