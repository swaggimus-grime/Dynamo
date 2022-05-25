#include "dynamopch.h"
#include "Shapes.h"
#include "Shader.h"

Cube::Cube(Graphics& g, std::shared_ptr<Shader> shader, const XMFLOAT3& pos, const XMFLOAT3& rot, const XMFLOAT3& scale)
{
	m_Shader = std::move(shader);

	VertexLayout layout;
	layout.AddAttrib("Pos", DXGI_FORMAT_R32G32B32_FLOAT);
	m_Mesh = std::make_unique<Mesh<Vertex>>(g, m_Vertices, m_Shader->GetVSCode(), layout, m_Indices);
}

void Cube::Render(Graphics& g)
{
	m_Shader->Bind(g);
	m_Mesh->Render(g);
}