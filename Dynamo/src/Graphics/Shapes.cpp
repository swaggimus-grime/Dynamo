#include "dynamopch.h"
#include "Shapes.h"
#include "Shader.h"

Shape3D::Shape3D(Graphics& g, const std::shared_ptr<Shader>& shader,
	const std::vector<XMFLOAT3>& vertices, const std::vector<UINT>& indices, const Transform& t)
	:Mesh(g, vertices, shader->GetVSCode(), VertexLayout({ { "Pos", DXGI_FORMAT_R32G32B32_FLOAT }}), indices),
	Transformable(t),
	m_Shader(std::move(shader))
{
}

void Shape3D::Render(Graphics& g)
{
	m_Shader->Bind(g);
	Mesh::Render(g);
}

Cube::Cube(Graphics& g, std::shared_ptr<Shader> shader, const Transform& t)
	:Shape3D(g, shader, m_Vertices, m_Indices, t)
{
}

Sphere::Sphere(Graphics& g, std::shared_ptr<Shader> shader, const Transform& t, float radius, UINT stacks, UINT sectors)
	:Shape3D(g, shader, std::move(CalculateVertices(radius, stacks, sectors)), std::move(CalculateIndices(stacks, sectors)), t)
{
}

// Sphere generation algorithm provided by Song Ho Ahn
// http://www.songho.ca/opengl/gl_sphere.html
std::vector<XMFLOAT3> Sphere::CalculateVertices(float radius, float stacks, float sectors)
{
	std::vector<XMFLOAT3> vertices;
	
	float phi = -M_PI / 2;
	for (UINT i = 0; i <= stacks; ++i)
	{
		float rcosphi = radius * cosf(phi); 
		float z = radius * sinf(phi);  
		phi += M_PI / stacks;

		float theta = 0.f;
		for (int j = 0; j <= sectors; ++j)
		{
			float x = rcosphi * cosf(theta);         
			float y = rcosphi * sinf(theta);         
			vertices.emplace_back(x, y, z);
			theta += 2 * M_PI / sectors;
		}
	}

	return vertices;
}

std::vector<UINT> Sphere::CalculateIndices(float stacks, float sectors)
{
	std::vector<UINT> indices;
	UINT k1, k2;
	for (UINT i = 0; i < stacks; i++)
	{
		k1 = i * (sectors + 1);
		k2 = k1 + sectors + 1;

		for (UINT j = 0; j < sectors; j++, k1++, k2++)
		{
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
			}

			if (i != (stacks - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
			}
		}
	}

	return indices;
}

Shape2D::Shape2D(Graphics& g, const std::shared_ptr<Shader>& shader,
	const std::vector<XMFLOAT2>& vertices, const std::vector<UINT>& indices, const Transform& t)
	: Mesh(g, vertices, shader->GetVSCode(), VertexLayout({ { "Pos", DXGI_FORMAT_R32G32_FLOAT } }), indices),
	Transformable(t),
	m_Shader(std::move(shader))
{
}

void Shape2D::Render(Graphics& g)
{
	m_Shader->Bind(g);
	Mesh::Render(g);
}

Plane::Plane(Graphics& g, std::shared_ptr<Shader> shader, const Transform& t)
	:Shape2D(g, shader, m_Vertices, m_Indices, t)
{
}
