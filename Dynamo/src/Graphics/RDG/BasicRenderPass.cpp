#include "dynamopch.h"
#include "BasicRenderPass.h"

#include "In.h"
#include "Out.h"
#include "Bindable/DSState.h"
#include "Bindable/Sampler.h"
#include "Bindable/Rasterizer.h"
#include "imgui.h"
#include <imgui_node_editor.h>

const std::string BasicRenderPass::s_Options[] = {"Nearest", "Trilinear", "Anisotropic"};

BasicRenderPass::BasicRenderPass(Graphics& g, const std::string& name)
	:RenderPass(std::move(name)), m_ShadowTransform(MakeShared<ShadowCamBuffer>(g)),
	m_ShadowSampler(MakeShared<ShadowSampler>(g)), 
	m_Sampler(Sampler::Evaluate(g, SAMPLER_MODE::NEAREST, 0, false)),
	m_Option(s_Options[0])
{
	AddBind(DSState::Evaluate(g));
	AddBind(m_ShadowTransform);
	AddBind(m_ShadowSampler);
	AddBind(m_Sampler);
	AddBind(Rasterizer::Evaluate(g, RS_MODE::CULL_NONE));
	//AddBind(std::make_shared<Sampler>(g, SAMPLER_MODE::ANISOTROPIC, 2, false));

	AddIn(BufferIn<RenderTarget>::Make("renderTarget", m_RT));
	AddIn(BufferIn<DepthStencil>::Make("depthStencil", m_DS));
	AddBindIn<Bindable>("shadowMap");
	AddOut(BufferOut<RenderTarget>::Make("renderTarget", m_RT));
	AddOut(BufferOut<DepthStencil>::Make("depthStencil", m_DS));
} 

void BasicRenderPass::Run(Graphics& g)
{
	g.SetCamera(*m_Camera);
	RenderPass::Run(g);
}

void BasicRenderPass::OnNodeSelect(Graphics& g)
{
	ImGui::Begin("Lambertian Pass");
	if (ImGui::BeginCombo("Filters", m_Option.c_str())) {
		for (int n = 0; n < std::size(s_Options); n++)
		{
			bool is_selected = m_Option == s_Options[n];
			if (ImGui::Selectable(s_Options[n].c_str(), is_selected)) {
				m_Option = s_Options[n];
				if (m_Option == "Nearest") {
					m_Sampler->SetMode(SAMPLER_MODE::NEAREST);
					m_ShadowSampler->SetBilinear(false);
				}
				else if (m_Option == "Trilinear") {
					m_Sampler->SetMode(SAMPLER_MODE::TRILINEAR);
					m_ShadowSampler->SetBilinear(true);
				}
				else if (m_Option == "Anisotropic") {
					m_Sampler->SetMode(SAMPLER_MODE::ANISOTROPIC);
				}
			}
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::Checkbox("Mipped", &g.Mipped());
	if ((g.Mipped() && !m_Sampler->Mipped()) || (!g.Mipped() && m_Sampler->Mipped())) {
		m_Sampler->SetMipped(g.Mipped());
	}
	/*ImGui::Checkbox("PCF", &g.HWPCF());
	if ((g.HWPCF() && !m_ShadowSampler->GetHwPcf()) || (!g.HWPCF() && m_ShadowSampler->GetHwPcf())) {
		m_ShadowSampler->SetHwPcf(g.HWPCF());
	}*/
	ImGui::Checkbox("Bilinear", &g.Bilinear());
	if ((g.Bilinear() && !m_ShadowSampler->GetBilinear()) || (!g.Bilinear() && m_ShadowSampler->GetBilinear())) {
		m_ShadowSampler->SetBilinear(g.Bilinear());
	}

	ImGui::End();
}
