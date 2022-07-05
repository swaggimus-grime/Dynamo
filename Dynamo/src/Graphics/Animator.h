#pragma once

#include <glm/glm.hpp>

class Animator
{
public:
	Animator(class Animation* animation);

	void UpdateAnimation(float dt);
	void PlayAnimation(Animation* pAnimation);

	void CalculateBoneTransform(const struct AssimpNodeData* node, glm::mat4 parentTransform);

	XMMATRIX* GetFinalBoneMatrices()
	{
		return m_FinalBoneMatrices;
	}

private:
	XMMATRIX m_FinalBoneMatrices[100];
    Animation* m_CurrentAnimation;
    float m_CurrentTime;
    float m_DeltaTime;
};