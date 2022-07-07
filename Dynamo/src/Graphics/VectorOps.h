#pragma once

#include <assimp/types.h>
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

static XMFLOAT3 cross(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	return { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
}

static FLOAT dot(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

static XMVECTOR aiToXMQuat(const aiQuaternion& q)
{
	return XMVectorSet(q.x, q.y, q.z, q.w);
}

static XMFLOAT3 aiToXMF3(const aiVector3D& v)
{
	return *reinterpret_cast<const XMFLOAT3*>(&v.x);
}

static XMMATRIX aiToXMMat4(const aiMatrix4x4& m)
{
	return *reinterpret_cast<const XMMATRIX*>(&m[0][0]);
}

static XMMATRIX XMMatrixTranslationFromF3(const XMFLOAT3& v)
{
	return XMMatrixTranslation(v.x, v.y, v.z);
}

static XMVECTOR aiToXMVec(const aiVector3D& v)
{
	return XMVectorSet(v.x, v.y, v.z, 0);
}