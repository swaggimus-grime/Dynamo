#pragma once

#include "Bone.h"

class Animator
{
public:
	Animator(class Animation* animation);

	void UpdateAnimation(float dt);
	void PlayAnimation(Animation* pAnimation);

	void CalculateBoneTransform(const struct AssimpNodeData* node, XMMATRIX parentTransform);

	inline const XMMATRIX* BoneMats() const
	{
		return m_FinalBoneMatrices;
	}

private:
	XMMATRIX m_FinalBoneMatrices[MAX_NUM_BONES];
    Animation* m_CurrentAnimation;
    float m_CurrentTime;
    float m_DeltaTime;
};