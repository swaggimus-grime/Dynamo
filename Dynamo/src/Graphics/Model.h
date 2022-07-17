#pragma once

class Model {
public:
	Model(class Graphics& g, const std::string& path);

private:
	std::vector<class Mesh> m_Meshes;
};