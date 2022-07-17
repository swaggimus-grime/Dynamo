//#include "Scene.h"
//#include "dynamopch.h"
//#include "Scene.h"
//#include <imgui.h>
//#include "Entity.h"
//
//Scene::Scene()
//{
//
//}
//
//void Scene::Render(Graphics& g)
//{
//	for (const auto& r : *this)
//		r.second->Render(g);
//}
//
//void Scene::RenderPos(Graphics& g)
//{
//	for (const auto& r : *this)
//		r.second->RenderPos(g);
//}
//
//void Scene::Submit(const std::string& name, std::shared_ptr<Entity> r)
//{
//	auto& it = find(name);
//	if (it != end())
//		it->second = r;
//	else
//		insert({ name, r });
//}
//
//
//void Scene::ShowGUI(Graphics& g)
//{
//	ImGui::Begin("Scene");
//	for (const auto& o :*this) {
//		if (ImGui::TreeNode(o.first.c_str()))
//		{
//			o.second->ShowGUI(g);
//			ImGui::TreePop();
//			ImGui::Separator();
//		}
//	}
//
//	ImGui::End();
//}