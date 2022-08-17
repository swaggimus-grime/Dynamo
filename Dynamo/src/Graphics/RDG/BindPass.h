#pragma once

#include "Pass.h"
#include "Bindable/Bindable.h"

class BindPass : public Pass {
public:
	BindPass(const std::string& name, const std::vector<Shared<Bindable>>& binds = {});
	void AddBind(Shared<Bindable> bind);
	void BindAll(Graphics& g);
	virtual void Finish() override;
	bool CheckFramebuffer() const { return m_RT || m_DS; }
	template<class T>
	void AddBindIn(const std::string& name)
	{
		size_t idx = m_Binds.size();
		m_Binds.emplace_back();
		AddIn(MakeUnique<VectorBindableIn<T>>(std::move(name), m_Binds, idx));
	}

private:
	void BindFramebuffer(Graphics& g);

protected:
	Shared<class RenderTarget> m_RT;
	Shared<class DepthStencil> m_DS;

private:
	std::vector<Shared<Bindable>> m_Binds;
};