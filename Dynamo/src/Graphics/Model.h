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
#include "Transformable.h"
#include "DSState.h"
#include "GUI/GUIable.h"

struct Vertex {
	XMFLOAT3 pos;
	XMFLOAT2 tex;
	XMFLOAT3 norm;
	XMFLOAT3 tan;
	XMFLOAT3 bitan;
};

class Model : public Renderable, public Transformable, public GUIable {
public:
	Model(Graphics& g, const std::string& path);
	~Model();
	virtual void Render(Graphics& g) override;
	virtual void ShowGUI() override;
public:

	class ModelException : public DynamoException {
	public:
		ModelException(const char* file, unsigned int line, const char* msg);
		virtual const char* GetType() const override;
		virtual const char* what() const override;
	};

private:
	std::shared_ptr<Texture2D> GetTexture(Graphics& g, const aiMaterial* mat, aiTextureType type, UINT slot);

	class ModelTransformBuffer : public ConstantBuffer {
	public:
		ModelTransformBuffer(Graphics& g);
		virtual void Bind(Graphics& g) override;
		inline void SetModel(const XMMATRIX& model) { m_Model = model; }
	private:
		struct {
			XMMATRIX Model;
			XMMATRIX ModelView;
			XMMATRIX MVP;
		} m_Transform;

		XMMATRIX m_Model;
	};
private:
	std::vector<Mesh<Vertex>> m_Meshes;
	std::shared_ptr<Shader> m_Shader;
	std::shared_ptr<DSState> m_DSState;
	std::unique_ptr<ModelTransformBuffer> m_TransformCBuff;
	UINT m_TotalVertices;
	UINT m_TotalIndices;
};

#define MODEL_EXCEP(msg) Model::ModelException(__FILE__, __LINE__, msg)

