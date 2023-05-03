#include "dynamopch.h"
#include "OutlineMaskPass.h"

#include "In.h"
#include "Out.h"
#include "Bindable/DepthStencil.h"
#include "Bindable/Shader.h"
#include "Bindable/DSState.h"
#include "Bindable/Rasterizer.h"

OutlineMaskPass::OutlineMaskPass(Graphics& g, const std::string& name)
	:RenderPass(std::move(name))
{
	AddIn(BufferIn<DepthStencil>::Make("depthStencil", m_DS));
	AddOut(BufferOut<DepthStencil>::Make("depthStencil", m_DS));
	AddBind(VertexShader::Evaluate(g, "res/shaders/Solidvs.hlsl"));
	AddBind(NullPixelShader::Evaluate(g));
	AddBind(DSState::Evaluate(g, DS_MODE::STENCIL_WRITE));
	AddBind(Rasterizer::Evaluate(g, RS_MODE::CULL_BACK));
}
