#pragma once

#include <functional>
#include "In.h"
#include "Out.h"
#include "Pass.h"

class RDG {
public:
	RDG(class Graphics& g);
	void Run(Graphics& g);
	void Clear();
	class RenderPass* GetRenderPass(const std::string& passName) const;
protected:
	inline void AddGlobalIn(Unique<In> in);
	inline void AddGlobalOut(Unique<Out> out);
	void AddPass(Unique<Pass> pass);
	void LinkPass(Pass& pass);
	void Target(const std::string& in, const std::string& target);
	Pass& FindPassByName(const std::string& name);
	void Finish();
	void LinkGlobalIns();

private:
	std::vector<Unique<Pass>> m_Passes;
	std::vector<Unique<In>> m_Ins;
	std::vector<Unique<Out>> m_Outs;
	Shared<class RenderTarget> m_BackBuff;
	Shared<class DepthStencil> m_MasterDS;
	bool m_Finished = false;
};

