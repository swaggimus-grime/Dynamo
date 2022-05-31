#include "dynamopch.h"
#include "Selector.h"

Selector::Selector(Graphics& g)
{
	m_Shader = std::make_unique<Shader>(g, L"res/shaders/Solidvs.cso", L"res/shaders/Solidps.cso");
	m_Write = std::make_unique<DSState>(g, DS_MODE::STENCIL_WRITE);
	m_Mask = std::make_unique<DSState>(g, DS_MODE::STENCIL_MASK);
	m_Default = std::make_unique<DSState>(g);
	m_Buff.Color = { 1.f, 0.f, 0.f, 1.f };
	m_SolidBuff = std::make_unique<ConstantBuffer>(g, SHADER_TYPE::PS, sizeof(m_Buff), 0, &m_Buff);

}

void Selector::Select(std::shared_ptr<Renderable> r)
{
	m_Selected = r;
}

void Selector::Render(Graphics& g)
{
	m_Write->Bind(g);
	m_Selected->Render(g);

	m_Mask->Bind(g);
	m_SolidBuff->Bind(g);
	m_Shader->Bind(g);
	m_Selected->RenderOutline(g);
	m_Default->Bind(g);
}
