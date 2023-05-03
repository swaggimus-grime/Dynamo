#pragma once

#include <assimp/quaternion.h>
#include <assimp/vector3.h>
#include <assimp/matrix4x4.h>
#include <DirectXMath.h>

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