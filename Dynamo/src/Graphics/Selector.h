#pragma once

#include "Renderable.h"
#include "Shader.h"
#include "DSState.h"
#include "Buffer.h"

class Selector {
public:
	Selector(Graphics& g);
	void Select(std::shared_ptr<Renderable> r);
	void Render(Graphics& g);
private:
	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<DSState> m_Write;
	std::unique_ptr<DSState> m_Mask;
	std::unique_ptr<DSState> m_Default;
	std::shared_ptr<Renderable> m_Selected;

private:
	struct SolidColor {
		XMFLOAT4 Color;
	} m_Buff;

	std::unique_ptr<ConstantBuffer> m_SolidBuff;
};