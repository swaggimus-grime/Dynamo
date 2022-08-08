#pragma once

class Work {
public:
	Work(class Step& step, class Renderable& renderable);
	void Run(class Graphics& g) const;

private:
	Step& m_Step;
	Renderable& m_Renderable;
};