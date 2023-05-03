#include "dynamopch.h"
#include "ShadowPass.h"

#include "In.h"
#include "Out.h"
#include "Bindable/DSState.h"
#include "Bindable/DepthStencil.h"
#include "Bindable/Shader.h"
#include "Bindable/BlendState.h"
#include "Bindable/Rasterizer.h"
#include "Bindable/Texture.h"
#include "Bindable/Viewport.h"
#include "imgui.h"

ShadowPass::ShadowPass(Graphics& g, const std::string& name)
	:RenderPass(std::move(name))
{
	m_DepthCube = MakeShared<DepthCubemap>(g, s_Size, 3);
	//AddBind(VertexShader::Evaluate(g, "res/shaders/Shadowvs.hlsl"));
	AddBind(NullPixelShader::Evaluate(g));
	AddBind(DSState::Evaluate(g, DS_MODE::STENCIL_OFF));
	AddBind(BlendState::Evaluate(g, BLEND_MODE::NONE));
	AddBind(MakeShared<Viewport>(g, (float)s_Size, (float)s_Size));
	AddBind(MakeShared<ShadowRasterizer>(g, 50, 2.0f, 0.1f));
	AddOut(BindableOut<DepthCubemap>::Make("shadowMap", m_DepthCube));

	DirectX::XMStoreFloat4x4(
		&m_Proj,
		DirectX::XMMatrixPerspectiveFovLH(M_PI / 2, 1.f, 0.5f, 100.f)
	);

	// +x
	DirectX::XMStoreFloat3(&m_CamDirs[0], DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));
	DirectX::XMStoreFloat3(&m_CamUps[0], DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	// -x
	DirectX::XMStoreFloat3(&m_CamDirs[1], DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f));
	DirectX::XMStoreFloat3(&m_CamUps[1], DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	// +y
	DirectX::XMStoreFloat3(&m_CamDirs[2], DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	DirectX::XMStoreFloat3(&m_CamUps[2], DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f));
	// -y
	DirectX::XMStoreFloat3(&m_CamDirs[3], DirectX::XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f));
	DirectX::XMStoreFloat3(&m_CamUps[3], DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
	// +z
	DirectX::XMStoreFloat3(&m_CamDirs[4], DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
	DirectX::XMStoreFloat3(&m_CamUps[4], DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	// -z
	DirectX::XMStoreFloat3(&m_CamDirs[5], DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f));
	DirectX::XMStoreFloat3(&m_CamUps[5], DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	// set the depth buffer to *something* so that the rg validation doesn't scream
	const_cast<ShadowPass*>(this)->m_DS = std::move(m_DepthCube->DepthBuff(0));
}

void ShadowPass::Run(Graphics& g)
{
	const auto pos = XMLoadFloat3(&m_Camera->GetPos());

	g.SetProjection(XMLoadFloat4x4(&m_Proj));
	for (size_t i = 0; i < 6; i++)
	{
		auto d = m_DepthCube->DepthBuff(i);
		d->Clear(g);
		const_cast<ShadowPass*>(this)->m_DS = std::move(d);
		const auto lookAt = pos + XMLoadFloat3(&m_CamDirs[i]);
		g.SetLookAt(XMMatrixLookAtLH(pos, lookAt, XMLoadFloat3(&m_CamUps[i])));

		/*const auto inv = XMMatrixInverse(nullptr, g.LookAt() * g.Projection());
		std::vector<XMFLOAT3> frustumCorners;
		for (unsigned int x = 0; x < 2; ++x)
		{
			for (unsigned int y = 0; y < 2; ++y)
			{
				for (unsigned int z = 0; z < 2; ++z)
				{
					auto pt =
						XMFLOAT4(
							2.0f * x - 1.0f,
							2.0f * y - 1.0f,
							2.0f * z - 1.0f,
							1.0f);
					XMStoreFloat4(&pt, XMVector4Transform(XMLoadFloat4(&pt), inv));
					pt.x /= pt.w;
					pt.y /= pt.w;
					pt.z /= pt.w;
					frustumCorners.push_back(XMFLOAT3(pt.x, pt.y, pt.z));
				}
			}
		}

		XMFLOAT3 center = XMFLOAT3(0, 0, 0);
		for (const auto& v : frustumCorners)
		{
			center += v;
		}
		center /= frustumCorners.size();
		g.SetLookAt(XMMatrixLookAtLH(XMLoadFloat3(&center) + XMLoadFloat3(&m_CamDirs[i]), XMLoadFloat3(&center), XMLoadFloat3(&m_CamUps[i])));*/


		RenderPass::Run(g);
	}
}
