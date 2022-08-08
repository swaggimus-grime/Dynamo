#include "dynamopch.h"
#include "BasicRDG.h"

#include "Graphics.h"
#include "ClearPass.h"
#include "BasicRenderPass.h"
#include "Pass.h"

BasicRDG::BasicRDG(Graphics& g)
	:RDG(g)
{
	{
		auto pass = MakeUnique<ClearPass>("clearRT");
		pass->Link("buffer", "#.backbuffer");
		AddPass(std::move(pass));
	}
	{
		auto pass = MakeUnique<ClearPass>("clearDS");
		pass->Link("buffer", "#.masterDepth");
		AddPass(std::move(pass));
	}
	{
		auto pass = MakeUnique<BasicRenderPass>(g, "BasicDraw");
		pass->Link("renderTarget", "clearRT.buffer");
		pass->Link("depthStencil", "clearDS.buffer");
		AddPass(std::move(pass));
	}

	Target("backbuffer", "BasicDraw.renderTarget");
	Finish();
}
