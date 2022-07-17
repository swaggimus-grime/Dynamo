#include "TestCube.h"
#include "dynamopch.h"
#include "TestCube.h"

#include "Step.h"
#include "Texture.h"
#include "Shapes.h"
#include "Sampler.h"
#include "Technique.h"
#include "Step.h"
#include "Shader.h"
#include "TransformBuffer.h"
#include "InputLayout.h"

TestCube::TestCube(Graphics& g, float size)
{
	auto& shape = Cube::Make();
	m_VBuff = MakeUnique<VertexBuffer>(g, shape.Vertices);
	m_IBuff = MakeUnique<IndexBuffer>(g, shape.Indices);
	m_Top = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	//auto tcb = std::make_shared<TransformBuffer>(g);

	{
		Technique lambertian;
		{
			Step only(0);
			//only.AddBind(Texture2D::Evaluate(g, "res\\skyboxes\\ocean\\0.jpg", 0));
			//only.AddBind(Sampler::Evaluate(g));
			auto& vs = VertexShader::Evaluate(g, "res\\shaders\\Solidvs.cso");
			only.AddBind(InputLayout::Evaluate(g, shape.Vertices, *vs));
			only.AddBind(vs);
			only.AddBind(PixelShader::Evaluate(g, "res\\shaders\\Solidps.cso"));
			only.AddBind(MakeShared<TransformBuffer>(g, *this));
			lambertian.AddStep(only);
		}

		AddTechnique(lambertian);
	}
}

void TestCube::SetPos(DirectX::XMFLOAT3 pos) noexcept
{
	this->pos = pos;
}

void TestCube::SetRotation(float roll, float pitch, float yaw) noexcept
{
	this->roll = roll;
	this->pitch = pitch;
	this->yaw = yaw;
}

XMMATRIX TestCube::ModelMat() const
{
	return DirectX::XMMatrixRotationRollPitchYaw(roll, pitch, yaw) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}

