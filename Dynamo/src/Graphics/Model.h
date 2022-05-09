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

struct Vertex {
	XMFLOAT3 pos;
	XMFLOAT3 norm;
	XMFLOAT2 tex;
	XMFLOAT3 tan;
	XMFLOAT3 bitan;
};

class Model : public Renderable {
public:
	Model(std::shared_ptr<GPU> gpu, const std::string& path, std::shared_ptr<Shader> shader);
	~Model();

public:
	struct Mesh {
		std::shared_ptr<VertexBuffer<Vertex>> VBuff;
		std::shared_ptr<IndexBuffer> IBuff;
		std::vector<std::shared_ptr<Texture2D>> Textures;

		Mesh(std::shared_ptr<VertexBuffer<Vertex>> vbuff, std::shared_ptr<IndexBuffer> ibuff,
			std::vector<std::shared_ptr<Texture2D>> textures)
			:VBuff(vbuff), IBuff(ibuff), Textures(textures)
		{
		}
	};

	class ModelException : public DynamoException {
	public:
		ModelException(const char* file, unsigned int line, const char* msg);
		virtual const char* GetType() const override;
		virtual const char* what() const override;
	};

private:
	std::shared_ptr<Texture2D> GetTexture(const aiMaterial* mat, aiTextureType type, UINT slot);
	virtual void Render() const override;
	virtual Shader& GetShader() const override { return *m_Shader; }
private:
	std::shared_ptr<GPU> m_GPU;
	std::vector<Mesh> m_Meshes;
	std::shared_ptr<Shader> m_Shader;
};

#define MODEL_EXCEP(msg) Model::ModelException(__FILE__, __LINE__, msg)

