#pragma once

#include <d3d11.h>
using namespace DirectX;

static void operator+=(XMFLOAT3& v1, const XMFLOAT3& v2)
{
	v1.x += v2.x;
	v1.y += v2.y;
	v1.z += v2.z;
}

static void operator/=(XMFLOAT3& v, float s)
{
	v.x /= s;
	v.y /= s;
	v.z /= s;
}

static XMFLOAT3 operator+(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
}

static XMFLOAT3 operator-(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	return { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
}

static XMFLOAT3 cross(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	return { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
}

static FLOAT dot(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}