#pragma once

#include "Mesh.h"

class Model : public Transformable {
public:
	Model(class Graphics& g, const std::string& path);
	void Submit();
	void LinkToRDG(class RDG& graph);
	virtual void ShowGUI() override;
	XMMATRIX ModelMat() const;
private:
	std::vector<Unique<Mesh>> m_Meshes;
};