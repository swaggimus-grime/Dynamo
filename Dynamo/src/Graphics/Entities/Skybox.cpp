#include "dynamopch.h"
#include "Skybox.h"

#include "Bindable/InputLayout.h"
#include "Bindable/Shader.h"
#include "Shapes.h"
#include "Bindable/Texture.h"
#include "Bindable/Sampler.h"
#include "Bindable/DSState.h"
#include "Bindable/Rasterizer.h"
#include "Bindable/Topology.h"

Skybox::Skybox(Graphics& g, const std::string& texDir)
{
	auto& cube = Cube::Make();
	m_VBuff = MakeUnique<VertexBuffer>(g, cube.Vertices);
	m_IBuff = MakeUnique<IndexBuffer>(g, cube.Indices);
	m_Top = Topology::Evaluate(g);

	{
		Technique lambertian("Shade");
		{
			Step only("BasicDraw");
			auto& vs = VertexShader::Evaluate(g, "res\\shaders\\Skyboxvs.cso");
			only.AddBind(InputLayout::Evaluate(g, cube.Vertices.Layout(), *vs));
			only.AddBind(vs);
			only.AddBind(PixelShader::Evaluate(g, "res\\shaders\\Skyboxps.cso"));
			only.AddBind(MakeShared<SkyboxCbuff>(g));
			only.AddBind(Cubemap::Evaluate(g, texDir));
			only.AddBind(Sampler::Evaluate(g));
			only.AddBind(DSState::Evaluate(g, DS_MODE::DEPTH_FIRST));
			only.AddBind(Rasterizer::Evaluate(g, RS_MODE::CULL_NONE));

			lambertian.AddStep(only);
		}

		AddTechnique(lambertian);
	}
}

XMMATRIX Skybox::ModelMat() const
{
	return XMMatrixIdentity();
}

Skybox::SkyboxCbuff::SkyboxCbuff(Graphics& g)
	:VertexConstantBuffer(g, 0)
{
}

void Skybox::SkyboxCbuff::Bind(Graphics& g)
{
	m_VP = XMMatrixTranspose(g.LookAt() * g.Projection());
	Update(g, &m_VP);
	__super::Bind(g);
}
