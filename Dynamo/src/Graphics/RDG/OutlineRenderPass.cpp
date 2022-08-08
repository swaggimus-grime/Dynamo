#include "OutlineRenderPass.h"

#include "In.h"
#include "Out.h"
#include "Bindable/Shader.h"
#include "Bindable/DSState.h"
#include "Bindable/Rasterizer.h"

OutlineRenderPass::OutlineRenderPass(Graphics& g, const std::string& name)
	:RenderPass(std::move(name))
{
	AddIn(BufferIn<RenderTarget>::Make("renderTarget", m_RT));
	AddIn(BufferIn<DepthStencil>::Make("depthStencil", m_DS));
	AddOut(BufferOut<RenderTarget>::Make("renderTarget", m_RT));
	AddOut(BufferOut<DepthStencil>::Make("depthStencil", m_DS));
	AddBind(VertexShader::Evaluate(g, "res/shaders/Solidvs.cso"));
	AddBind(PixelShader::Evaluate(g, "res/shaders/Solidps.cso"));
	AddBind(DSState::Evaluate(g, DS_MODE::STENCIL_MASK));
	AddBind(Rasterizer::Evaluate(g, RS_MODE::CULL_BACK));
}