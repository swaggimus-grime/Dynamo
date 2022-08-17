#include "dynamopch.h"
#include "ClearPass.h"

#include "Bindable/Buffer.h"
#include "In.h"
#include "Out.h"

ClearPass::ClearPass(const std::string& name)
	:Pass(std::move(name))
{
	AddIn(BufferIn<Buffer>::Make("buffer", m_Buffer));
	AddOut(BufferOut<Buffer>::Make("buffer", m_Buffer));
}

void ClearPass::Run(Graphics& g)
{
	if(m_Buffer)
		m_Buffer->Clear(g);
}
