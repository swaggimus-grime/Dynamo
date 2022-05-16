#pragma once

#include "Debug/DynamoException.h"
#include "Buffer.h"
#include "Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.h" 
#include "Renderable.h"
#include "Graphics.h"
#include "Mesh.h"

struct Vertex {
	XMFLOAT3 pos;
	XMFLOAT3 norm;
	XMFLOAT2 tex;
	XMFLOAT3 tan;
	XMFLOAT3 bitan;
};

class Model : public Renderable {
public:
	Model(Graphics& g, const std::string& path, std::shared_ptr<Shader> shader);
	~Model();
	virtual void Render(Graphics& g) const override;
public:

	class ModelException : public DynamoException {
	public:
		ModelException(const char* file, unsigned int line, const char* msg);
		virtual const char* GetType() const override;
		virtual const char* what() const override;
	};

private:
	std::shared_ptr<Texture2D> GetTexture(Graphics& g, const aiMaterial* mat, aiTextureType type, UINT slot);
	
private:
	std::vector<Mesh<Vertex>> m_Meshes;
	std::shared_ptr<Shader> m_Shader;
};

#define MODEL_EXCEP(msg) Model::ModelException(__FILE__, __LINE__, msg)

