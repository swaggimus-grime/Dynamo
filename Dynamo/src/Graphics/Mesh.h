#pragma once

#include "Renderable.h"
#include <assimp/scene.h>
#include "VertexBuffer.h"
#include "Texture.h"

class Mesh : public Renderable {
public:
	Mesh(Graphics& g, const std::string& directory, const aiMesh* mesh, const aiMaterial* mat);
	virtual XMMATRIX ModelMat() const { return XMMatrixIdentity(); }
private:
	Unique<Texture2D> GetTexture(Graphics& g, const aiMaterial* mat, aiTextureType type, UINT slot);

private:
	std::vector<Unique<Texture2D>> m_Textures;
	static VertexLayout m_Layout;
	std::string m_Directory;
};