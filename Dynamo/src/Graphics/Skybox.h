#pragma once

#include <d3d11.h>
#include "Bindable.h"
#include "Renderable.h"
#include "Mesh.h"
#include "Shader.h"
#include "Shapes.h"
#include "GUI/GUIable.h"

class Cubemap : public Texture2D {
public:
	Cubemap(Graphics& g, const std::wstring& texDir, UINT slot);
};

class Skybox : public Renderable, public D3DGUIable {
public:
	Skybox(Graphics& g, const std::wstring& texDir);
	virtual void Render(Graphics& g) override;
	virtual void ShowGUI(Graphics& g) override;
private:
	std::vector<std::shared_ptr<Bindable>> m_Bindables;
	std::unique_ptr<Cube> m_Cube;

private:
	struct SkyboxTransform {
		XMMATRIX VP;
	} m_Transform;

	class SkyboxConstantBuffer : public ConstantBuffer {
	public:
		SkyboxConstantBuffer(Graphics& g, SkyboxTransform& transformRef, SHADER_TYPE type, SIZE_T size);
		virtual void Bind(Graphics& g) override;

	private:
		SkyboxTransform& m_TransormRef;
	};
};