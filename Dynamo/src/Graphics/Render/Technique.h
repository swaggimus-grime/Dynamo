#pragma once

#include "Step.h"

class Technique {
public:
	Technique(const std::string& name);
	inline std::string Name() const { return m_Name; }
	void Submit(class Renderable& r);
	void LinkToRDG(class RDG& graph);
	void AddStep(const Step& step);
	void SetParent(const Renderable& parent);
	
private:
	std::string m_Name;
	std::vector<Step> m_Steps;
};