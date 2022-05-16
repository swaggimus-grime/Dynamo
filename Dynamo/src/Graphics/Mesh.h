#pragma once

#include "Buffer.h"
#include "Texture.h"

template<class T>
struct Mesh {
	std::shared_ptr<VertexBuffer<T>> VBuff;
	std::shared_ptr<IndexBuffer> IBuff;
	std::vector<std::shared_ptr<Texture2D>> Textures;

	Mesh(std::shared_ptr<VertexBuffer<T>> vbuff, std::shared_ptr<IndexBuffer> ibuff,
		std::vector<std::shared_ptr<Texture2D>> textures)
		:VBuff(vbuff), IBuff(ibuff), Textures(textures)
	{
	}

	Mesh() = default;
};