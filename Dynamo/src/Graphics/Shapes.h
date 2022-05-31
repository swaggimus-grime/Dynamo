#pragma once

#include "Renderable.h"
#include "Transformable.h"
#include "Mesh.h"
#include "Shader.h"

class Shape : public Transformable, public Renderable {
public:
	Shape(const Transform& t);
	virtual void Render(Graphics& g) = 0;
protected:
	std::shared_ptr<Shader> m_Shader;
};

class Cube : public Shape {
public:
	Cube(Graphics& g, std::shared_ptr<Shader> shader, const Transform& t);
	inline void AddTexture(std::shared_ptr<Texture2D> tex) { m_Mesh->AddTexture(std::move(tex)); }
	virtual void Render(Graphics& g) override;
	virtual void RenderOutline(Graphics& g) override;
	inline std::shared_ptr<Texture2D>& GetTexture(UINT slot) { return m_Mesh->GetTexture(slot); }
private:
	static struct Vertex {
		XMFLOAT3 Pos;

	};

	inline static const std::vector<Vertex> m_Vertices = {
		{{ -.5f, -.5f, -.5f }},
		{{  .5f, -.5f, -.5f }},
		{{ -.5f,  .5f, -.5f }},
		{{  .5f,  .5f, -.5f }},
		{{ -.5f, -.5f,  .5f }},
		{{  .5f, -.5f,  .5f }},
		{{ -.5f,  .5f,  .5f }},
		{{  .5f,  .5f,  .5f }}
	};

	inline static const std::vector<UINT> m_Indices = {
		0, 2 ,1, 2 ,3, 1,
		1, 3 ,5, 3 ,7, 5,
		2, 6 ,3, 3 ,6, 7,
		4, 5 ,7, 4 ,7, 6,
		0, 4 ,2, 2 ,4, 6,
		0, 1 ,4, 1 ,5, 4
	};

	std::unique_ptr<Mesh<Vertex>> m_Mesh;
};