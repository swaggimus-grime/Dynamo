#pragma once

#include "Mesh.h"

class Model {
public:
	Model(class Graphics& g, const std::string& path);
	void Submit();
	void LinkToRDG(class RDG& graph);
	void SetPos(const XMFLOAT3& pos);
	void SetRot(const XMFLOAT3& rot);
	void SetScale(const XMFLOAT3& scale);
	void ShowGUI();

private:
	std::vector<Unique<Mesh>> m_Meshes;
};