#pragma once

#include "AnimMesh.h"
#include "Bindable/Sampler.h"
#include "Animation/Bone.h"
#include "Animation/Animator.h"
#include "Animation/Animation.h"

class AnimModel : public Transformable {
public:
	AnimModel(class Graphics& g, const std::string& path, bool leftHanded);
	void Submit();
	void LinkToRDG(class RDG& graph);
	void Update(float dt);
	virtual void ShowGUI(Graphics& g) override;
	XMMATRIX ModelMat() const;
	inline auto& Bones() { return m_Bones; }
	inline int& BoneCounter() { return m_BoneCounter; }
	inline const XMMATRIX* BoneMats() const { return m_Animator->BoneMats(); }

private:
	std::vector<Unique<AnimMesh>> m_Meshes;
	std::map<std::string, struct BoneInfo> m_Bones;
	int m_BoneCounter = 0;
	Unique<class Animator> m_Animator;
	Unique<class Animation> m_Animation;
};