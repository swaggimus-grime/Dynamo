#pragma once

#include "Render/Renderable.h"
#include <assimp/scene.h>
#include "Bindable/VertexBuffer.h"
#include "Bindable/Texture.h"
#include "Bindable/TransformBuffer.h"

class Model;

class Mesh : public Renderable {
public:
	Mesh(Graphics& g, Model* parent, const std::string& directory, const aiMesh* mesh, const aiMaterial* mat, float scale);
	virtual XMMATRIX ModelMat() const;
	virtual void Bind(Graphics& g) const override;

private:
	Shared<Texture2D> GetTexture(Graphics& g, const aiMaterial* mat, aiTextureType type, UINT slot);

private:
	Model* m_Parent;
	std::vector<Shared<Texture2D>> m_Textures;
	Unique<VertexLayout> m_Layout;
	std::string m_Directory;
};