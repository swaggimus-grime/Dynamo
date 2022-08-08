#include "dynamopch.h"
#include "OutlineRDG.h"

#include "ClearPass.h"
#include "BasicRenderPass.h"
#include "OutlineMaskPass.h"
#include "OutlineRenderPass.h"
#include "SkyboxPass.h"
#include "Entities/Camera.h"

OutlineRDG::OutlineRDG(Graphics& g)
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
		auto pass = MakeUnique<BasicRenderPass>(g, "lambertian");
		pass->Link("renderTarget", "clearRT.buffer");
		pass->Link("depthStencil", "clearDS.buffer");
		AddPass(std::move(pass));
	}
	{
		auto pass = MakeUnique<SkyboxPass>(g, "skybox");
		pass->Link("renderTarget", "lambertian.renderTarget");
		pass->Link("depthStencil", "lambertian.depthStencil");
		AddPass(std::move(pass));
	}
	{
		auto pass = MakeUnique<OutlineMaskPass>(g, "outlineMask");
		pass->Link("depthStencil", "skybox.depthStencil");
		AddPass(std::move(pass));
	}
	{
		auto pass = MakeUnique<OutlineRenderPass>(g, "outlineDraw");
		pass->Link("renderTarget", "skybox.renderTarget");
		pass->Link("depthStencil", "outlineMask.depthStencil");
		AddPass(std::move(pass));
	}

	Target("backbuffer", "outlineDraw.renderTarget");
	Finish();
}

void OutlineRDG::SetCamera(Camera& camera)
{
	dynamic_cast<BasicRenderPass&>(FindPassByName("lambertian")).SetCamera(camera);
	dynamic_cast<SkyboxPass&>(FindPassByName("skybox")).SetCamera(camera);
}
