#pragma once

#include <d3d11.h>
using namespace DirectX;

static XMFLOAT3 operator+(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

static XMFLOAT3 operator-(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}