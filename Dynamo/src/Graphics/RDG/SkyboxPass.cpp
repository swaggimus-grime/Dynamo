#include "dynamopch.h"
#include "SkyboxPass.h"

#include "In.h"
#include "Out.h"
#include "Bindable/DSState.h"
#include "Bindable/Rasterizer.h"
#include "Bindable/Shader.h"
#include "Bindable/Topology.h"
#include "Bindable/Texture.h"
#include "Bindable/Sampler.h"
#include "Bindable/VertexBuffer.h"
#include "Bindable/IndexBuffer.h"
#include "Graphics.h"
#include "Entities/Shapes.h"

SkyboxPass::SkyboxPass(Graphics& g, const std::string& name)
	:BindPass(std::move(name))
{
	AddIn(BufferIn<RenderTarget>::Make("renderTarget", m_RT));
	AddIn(BufferIn<DepthStencil>::Make("depthStencil", m_DS));
	AddBind(MakeShared<Cubemap>(g, "res\\skyboxes\\yokohama"));
	AddBind(DSState::Evaluate(g, DS_MODE::DEPTH_FIRST));
	AddBind(Sampler::Evaluate(g, SAMPLER_MODE::LINEAR_WRAP));
	AddBind(Rasterizer::Evaluate(g, RS_MODE::CULL_NONE));
	AddBind(MakeShared<SkyboxCBuff>(g));
	AddBind(PixelShader::Evaluate(g, "res/shaders/Skyboxps.cso"));
	AddBind(Topology::Evaluate(g, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	{ // geometry-related
		auto pvs = VertexShader::Evaluate(g, "res/shaders/Skyboxvs.cso");
		{ // cube
			auto model = Cube::Make();
			const auto geometryTag = "$cube_map";
			m_VBuff = VertexBuffer::Evaluate(g, geometryTag, model.Vertices);
			m_IBuff = IndexBuffer::Evaluate(g, geometryTag, model.Indices);
			m_CubeCount = (UINT)model.Indices.size();
			// layout is shared between cube and sphere; use cube data to generate
			AddBind(InputLayout::Evaluate(g, model.Vertices.Layout(), *pvs));
		}
		//{ // sphere
		//	auto model = Sphere::Make();
		//	const auto geometryTag = "$sphere_map";
		//	pSphereVertices = VertexBuffer::Evaluate(g, geometryTag, std::move(model.vertices));
		//	pSphereIndices = IndexBuffer::Evaluate(g, geometryTag, std::move(model.indices));
		//	sphereCount = (UINT)model.indices.size();
		//}
		AddBind(std::move(pvs));
	}
	AddOut(BufferOut<RenderTarget>::Make("renderTarget", m_RT));
	AddOut(BufferOut<DepthStencil>::Make("depthStencil", m_DS));
}

void SkyboxPass::Run(Graphics& g)
{
	if (!CheckFramebuffer())
		return;
	DYNAMO_ASSERT(m_Camera, "Skybox needs reference to a camera");
	UINT indexCount;
	g.SetCamera(*m_Camera);

	m_VBuff->Bind(g);
	m_IBuff->Bind(g);
	indexCount = m_CubeCount;
	
	BindAll(g);
	g.DC().DrawIndexed(indexCount, 0, 0);
}

SkyboxPass::SkyboxCBuff::SkyboxCBuff(Graphics& g)
	:VertexConstantBuffer<XMMATRIX>(g, 0)
{
}

void SkyboxPass::SkyboxCBuff::Bind(Graphics& g)
{
	XMMATRIX vp = XMMatrixTranspose(g.LookAt() * g.Projection());
	Update(g, &vp);
	VertexConstantBuffer<XMMATRIX>::Bind(g);
}
