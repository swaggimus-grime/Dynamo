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
#include "BlendState.h"
#include "GUI/GUIable.h"
#include "Animation.h"
#include "Animator.h"

struct SkinVertex {
	XMFLOAT3 pos;
	XMFLOAT2 tex;
	XMFLOAT3 norm;
	XMFLOAT3 tan;
	XMFLOAT3 bitan;
	XMINT4 bones;
	XMFLOAT4 weights;
};

class AnimModel : public Renderable, public Transformable {
public:
	AnimModel(Graphics& g, const std::string& path, const Transform& t);
	~AnimModel();
	void Reload(Graphics& g, const std::string& path);
	virtual void Render(Graphics& g) override;
	virtual void ShowGUI(Graphics& g) override;
	void Animate(Graphics& g, float deltaTime);
	auto& GetBoneInfoMap() { return m_Bones; }
	int& GetBoneCount() { return m_BoneCounter; }
public:

	class ModelException : public DynamoException {
	public:
		ModelException(const char* file, unsigned int line, const char* msg);
		virtual const char* GetType() const override;
		virtual const char* what() const override;
	};

private:
	void SetVertexBoneData(SkinVertex& vertex, int boneID, float weight);

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

	struct Material {
		float SpecIntensity;
		float SpecPower;
	};

	class MaterialBuffer : public ConstantBuffer {
	public:
		MaterialBuffer(Graphics& g, const Material& mat);

	};
private:
	std::vector<Mesh<SkinVertex>> m_Meshes;
	std::shared_ptr<Shader> m_Shader;
	std::unique_ptr<ConstantBuffer> m_BoneBuff;
	std::unique_ptr<ModelTransformBuffer> m_TransformCBuff;
	std::unique_ptr<MaterialBuffer> m_MatCBuff;
	std::unique_ptr<Sampler> m_Samp;
	std::unique_ptr<BlendState> m_Blend;
	UINT m_TotalVertices;
	UINT m_TotalIndices;

	std::map<std::string, struct BoneInfo> m_Bones;
	int m_BoneCounter = 0;
	std::unique_ptr<Animator> m_Animator;
	std::unique_ptr<Animation> m_Animation;
};

#define ANIM_MODEL_EXCEP(msg) AnimModel::ModelException(__FILE__, __LINE__, msg)
