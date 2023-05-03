#pragma once

#include "Mesh.h"
#include "Bindable/Sampler.h"
#include "Animation/Bone.h"
#include "Animation/Animator.h"
#include "Animation/Animation.h"

class Model : public Transformable {
public:
	Model(class Graphics& g, const std::string& path, float scale = 1.f);
	void Submit();
	void LinkToRDG(class RDG& graph);
	virtual void ShowGUI(Graphics& g) override;
	XMMATRIX ModelMat() const;


private:
	std::vector<Unique<Mesh>> m_Meshes;

};