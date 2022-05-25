#pragma once

#include "Renderable.h"
#include <unordered_map>
#include "GUI/GUIable.h"

class Scene : public D3DGUIable, public GUIable {
public:
	Scene();
	void Submit(std::shared_ptr<D3DGUIable> g, const std::string& name);
	void Submit(std::shared_ptr<GUIable> g, const std::string& name);
	virtual void ShowGUI(class Graphics& g) override;

private:
	virtual void ShowGUI() override;
private:
	std::unordered_map<std::string, std::shared_ptr<GUIable>> m_GUIables;
	std::unordered_map<std::string, std::shared_ptr<D3DGUIable>> m_D3DGUIables;
};