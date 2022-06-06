#pragma once

#include "Renderable.h"
#include "Transformable.h"
#include "Mesh.h"
#include "Shader.h"

class Shape3D : public Mesh<XMFLOAT3>, public Transformable {
public:
	Shape3D(Graphics& g, const std::shared_ptr<Shader>& shader,
		const std::vector<XMFLOAT3>& vertices, const std::vector<UINT>& indices, const Transform& t);
	virtual void Render(Graphics& g) override;
protected:
	std::shared_ptr<Shader> m_Shader;
};

class Sphere : public Shape3D {
public:
	Sphere(Graphics& g, std::shared_ptr<Shader> shader, const Transform& t, float radius = 1.f, UINT stacks = 18, UINT sectors = 36);

private:
	std::vector<XMFLOAT3> CalculateVertices(float radius, float stacks, float sectors);
	std::vector<UINT> CalculateIndices(float stacks, float sectors);
};

class Cube : public Shape3D {
public:
	Cube(Graphics& g, std::shared_ptr<Shader> shader, const Transform& t);

private:
	inline static const std::vector<XMFLOAT3> m_Vertices = {
		{ -.5f, -.5f, -.5f },
		{  .5f, -.5f, -.5f },
		{ -.5f,  .5f, -.5f },
		{  .5f,  .5f, -.5f },
		{ -.5f, -.5f,  .5f },
		{  .5f, -.5f,  .5f },
		{ -.5f,  .5f,  .5f },
		{  .5f,  .5f,  .5f }
	};

	inline static const std::vector<UINT> m_Indices = {
		0, 2 ,1, 2 ,3, 1,
		1, 3 ,5, 3 ,7, 5,
		2, 6 ,3, 3 ,6, 7,
		4, 5 ,7, 4 ,7, 6,
		0, 4 ,2, 2 ,4, 6,
		0, 1 ,4, 1 ,5, 4
	};
};

class Shape2D : public Mesh<XMFLOAT2>, public Transformable {
public:
	Shape2D(Graphics& g, const std::shared_ptr<Shader>& shader,
		const std::vector<XMFLOAT2>& vertices, const std::vector<UINT>& indices, const Transform& t);
	virtual void Render(Graphics& g) override;
protected:
	std::shared_ptr<Shader> m_Shader;
};

class Plane : public Shape2D {
public:
	Plane(Graphics& g, std::shared_ptr<Shader> shader, const Transform& t);

private:
	inline static const std::vector<XMFLOAT2> m_Vertices = {
		{-1, 1},
		{1, 1},
		{-1, -1},
		{1, -1}
	};

	inline static const std::vector<UINT> m_Indices = {
		0, 1, 2, 1, 3, 2
	};
};