#pragma once

#include "Renderable.h"
#include <unordered_map>
#include "GUI/GUIable.h"

class Scene : public Renderable {
public:
	Scene();
	virtual void Render(Graphics& g) override;
	void Submit(const std::string& name, std::shared_ptr<Renderable> r);
	virtual void ShowGUI(Graphics& g) override;

private:
	std::unordered_map<std::string, std::shared_ptr<Renderable>> m_Objects;
};