#pragma once

class Step {
public:
	Step(const std::string& passName);
	void Submit(class Renderable& r);
	void Bind(class Graphics& g) const;
	void AddBind(Shared<class Bindable> bind);
	void LinkToRDG(class RDG& graph);
	void SetParent(const Renderable& parent);
private:
	class RenderPass* m_Pass = nullptr;
	std::string m_PassName;
	std::vector<Shared<Bindable>> m_Binds;
};