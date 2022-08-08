//#pragma once
//
//#include "Renderable.h"
//#include <unordered_map>
//
//class Scene : public Renderable, public std::unordered_map<std::string, std::shared_ptr<class Entity>> {
//public:
//	Scene();
//	virtual void Render(Graphics& g) override;
//	virtual void RenderPos(Graphics& g) override;
//	void Submit(const std::string& name, std::shared_ptr<Entity> r);
//	void ShowGUI(Graphics& g);
//};