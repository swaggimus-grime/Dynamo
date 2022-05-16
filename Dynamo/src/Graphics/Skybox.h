#pragma once

#include <d3d11.h>
#include "Bindable.h"
#include "Renderable.h"
#include "Mesh.h"
#include "Shader.h"

class Cubemap : public Texture2D {
public:
	Cubemap(Graphics& g, const std::wstring& texDir, UINT slot);
};

class Skybox : public Renderable {
public:
	Skybox(Graphics& g, const std::wstring& texDir, std::shared_ptr<Shader> shader);
	virtual void Render(Graphics& g) const override;
private:
	std::unique_ptr<Cubemap> m_Cubemap;
	std::shared_ptr<Shader> m_Shader;

	struct CubeVertex {
		XMFLOAT3 Pos;
		XMFLOAT2 Tex;
	};

	Mesh<XMFLOAT3> m_Cube;
};