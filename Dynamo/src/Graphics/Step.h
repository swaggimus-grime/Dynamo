#pragma once

class Step {
public:
	Step(UINT passIdx);
	void Submit(class Graphics& g, class Renderable& r);
	void Bind(Graphics& g) const;
	void AddBind(Shared<class Bindable> bind);

private:
	UINT m_PassIdx;
	std::vector<Shared<Bindable>> m_Binds;
};