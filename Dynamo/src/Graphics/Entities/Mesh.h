#pragma once

#include "Render/Renderable.h"
#include <assimp/scene.h>
#include "Bindable/VertexBuffer.h"
#include "Bindable/Texture.h"
#include "Bindable/TransformBuffer.h"

class Mesh : public Renderable, public Transformable {
public:
	Mesh(Graphics& g, const std::string& directory, const aiMesh* mesh, const aiMaterial* mat);
	virtual XMMATRIX ModelMat() const { return TransformMat(); }
private:
	Shared<Texture2D> GetTexture(Graphics& g, const aiMaterial* mat, aiTextureType type, UINT slot);

private:
	std::vector<Shared<Texture2D>> m_Textures;
	static VertexLayout m_Layout;
	std::string m_Directory;
};