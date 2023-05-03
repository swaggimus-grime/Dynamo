#pragma once

#include "Bindable/VertexBuffer.h"

struct Shape {
	VertexData Vertices;
	std::vector<UINT> Indices;

	void Transform(XMMATRIX matrix)
	{
		for (int i = 0; i < Vertices.NumVertices(); i++)
		{
			auto& pos = Vertices.Pos(i);
			XMStoreFloat3(
				&pos,
				XMVector3Transform(XMLoadFloat3(&pos), matrix)
			);
		}
	}

	void SetNormalsIndependentFlat() 
	{
		using namespace DirectX;
		for (size_t i = 0; i < Indices.size(); i += 3)
		{
			const auto v0 = Vertices.Pos(Indices[i]);
			const auto v1 = Vertices.Pos(Indices[i + 1]);
			const auto v2 = Vertices.Pos(Indices[i + 2]);

			const auto p0 = XMLoadFloat3(&v0);
			const auto p1 = XMLoadFloat3(&v1);
			const auto p2 = XMLoadFloat3(&v2);
			const auto n = XMVector3Normalize(XMVector3Cross((p1 - p0), (p2 - p0)));

			XMStoreFloat3(&Vertices.Pos(Indices[i]), n);
			XMStoreFloat3(&Vertices.Pos(Indices[i + 1]), n);
			XMStoreFloat3(&Vertices.Pos(Indices[i + 2]), n);
		}
	}
};

class Cube
{
public:
	static Shape Make()
	{
		constexpr float side = 1.0f / 2.0f;

		VertexLayout layout(ATTRIB_POS);
		VertexData vertices(layout, 8);
		vertices.Pos(0) = { -side,-side,-side };
		vertices.Pos(1) = { side,-side,-side };
		vertices.Pos(2) = { -side,side,-side };
		vertices.Pos(3) = { side,side,-side };
		vertices.Pos(4) = { -side,-side,side };
		vertices.Pos(5) = { side,-side,side };
		vertices.Pos(6) = { -side,side,side };
		vertices.Pos(7) = { side,side,side };

		return{
			std::move(vertices), {
				0,2,1, 2,3,1,
				1,3,5, 3,7,5,
				2,6,3, 3,6,7,
				4,5,7, 4,7,6,
				0,4,2, 2,4,6,
				0,1,4, 1,5,4
			}
		};
	}

	static Shape MakeIndependent(ATTRIB_FLAGS flags)
	{

		constexpr float side = 1.0f / 2.0f;

		VertexLayout layout(flags);
		VertexData vertices(layout, 24);
		vertices.Pos(0) = { -side,-side,-side };// 0 near side
		vertices.Pos(1) = { side,-side,-side };// 1
		vertices.Pos(2) = { -side,side,-side };// 2
		vertices.Pos(3) = { side,side,-side };// 3
		vertices.Pos(4) = { -side,-side,side };// 4 far side
		vertices.Pos(5) = { side,-side,side };// 5
		vertices.Pos(6) = { -side,side,side };// 6
		vertices.Pos(7) = { side,side,side };// 7
		vertices.Pos(8) = { -side,-side,-side };// 8 left side
		vertices.Pos(9) = { -side,side,-side };// 9
		vertices.Pos(10) = { -side,-side,side };// 10
		vertices.Pos(11) = { -side,side,side };// 11
		vertices.Pos(12) = { side,-side,-side };// 12 right side
		vertices.Pos(13) = { side,side,-side };// 13
		vertices.Pos(14) = { side,-side,side };// 14
		vertices.Pos(15) = { side,side,side };// 15
		vertices.Pos(16) = { -side,-side,-side };// 16 bottom side
		vertices.Pos(17) = { side,-side,-side };// 17
		vertices.Pos(18) = { -side,-side,side };// 18
		vertices.Pos(19) = { side,-side,side };// 19
		vertices.Pos(20) = { -side,side,-side };// 20 top side
		vertices.Pos(21) = { side,side,-side };// 21
		vertices.Pos(22) = { -side,side,side };// 22
		vertices.Pos(23) = { side,side,side };// 23

		return{
			std::move(vertices),{
				0,2, 1,    2,3,1,
				4,5, 7,    4,7,6,
				8,10, 9,  10,11,9,
				12,13,15, 12,15,14,
				16,17,18, 18,17,19,
				20,23,21, 20,22,23
			}
		};
	}

	static Shape MakeIndependentTextured()
	{
		VertexLayout layout(ATTRIB_POS | ATTRIB_TEX | ATTRIB_NORM);
		VertexData vertices(layout, 24);
		vertices.Tex(0) = { 0.0f,0.0f };
		vertices.Tex(1) = { 1.0f,0.0f };
		vertices.Tex(2) = { 0.0f,1.0f };
		vertices.Tex(3) = { 1.0f,1.0f };
		vertices.Tex(4) = { 0.0f,0.0f };
		vertices.Tex(5) = { 1.0f,0.0f };
		vertices.Tex(6) = { 0.0f,1.0f };
		vertices.Tex(7) = { 1.0f,1.0f };
		vertices.Tex(8) = { 0.0f,0.0f };
		vertices.Tex(9) = { 1.0f,0.0f };
		vertices.Tex(10) = { 0.0f,1.0f };
		vertices.Tex(11) = { 1.0f,1.0f };
		vertices.Tex(12) = { 0.0f,0.0f };
		vertices.Tex(13) = { 1.0f,0.0f };
		vertices.Tex(14) = { 0.0f,1.0f };
		vertices.Tex(15) = { 1.0f,1.0f };
		vertices.Tex(16) = { 0.0f,0.0f };
		vertices.Tex(17) = { 1.0f,0.0f };
		vertices.Tex(18) = { 0.0f,1.0f };
		vertices.Tex(19) = { 1.0f,1.0f };
		vertices.Tex(20) = { 0.0f,0.0f };
		vertices.Tex(21) = { 1.0f,0.0f };
		vertices.Tex(22) = { 0.0f,1.0f };
		vertices.Tex(23) = { 1.0f,1.0f };

		return{
			std::move(vertices),{
				0,2, 1,    2,3,1,
				4,5, 7,    4,7,6,
				8,10, 9,  10,11,9,
				12,13,15, 12,15,14,
				16,17,18, 18,17,19,
				20,23,21, 20,22,23
			}
		};
	}
};

class Plane {
public:
	static Shape MakeIndependentTextured()
	{
		constexpr float side = 1.0f / 2.0f;
		VertexLayout layout(ATTRIB_POS | ATTRIB_TEX | ATTRIB_NORM);
		VertexData vertices(layout, 24);
		vertices.Pos(0) = { -side,-side,0 };// 0 near side
		vertices.Pos(1) = { side,-side,0 };// 1
		vertices.Pos(2) = { -side,side,0 };// 2
		vertices.Pos(3) = { side,side,0 };// 3
		vertices.Tex(0) = { 0.0f,0.0f };
		vertices.Tex(1) = { 1.0f,0.0f };
		vertices.Tex(2) = { 0.0f,1.0f };
		vertices.Tex(3) = { 1.0f,1.0f };
		vertices.Norm(0) = { 0.f, 0.f, 1.f };
		vertices.Norm(1) = { 0.f, 0.f, 1.f };
		vertices.Norm(2) = { 0.f, 0.f, 1.f };
		vertices.Norm(3) = { 0.f, 0.f, 1.f };

		return{
			std::move(vertices),{
				0,2, 1,    2,3,1,
			}
		};
	}
};