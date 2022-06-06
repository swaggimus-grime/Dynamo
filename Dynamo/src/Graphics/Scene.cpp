#include "Scene.h"
#include "dynamopch.h"
#include "Scene.h"
#include <imgui.h>

Scene::Scene()
{

}

void Scene::Render(Graphics& g)
{
	for (const auto& r : m_Objects)
		r.second->Render(g);
}

void Scene::Submit(const std::string& name, std::shared_ptr<Renderable> r)
{
	auto& it = m_Objects.find(name);
	if (it != m_Objects.end())
		it->second = r;
	else
		m_Objects.insert({ name, r });
}


void Scene::ShowGUI(Graphics& g)
{
	ImGui::Begin("Scene");
	for (const auto& o : m_Objects) {
		if (ImGui::TreeNode(o.first.c_str()))
		{
			o.second->ShowGUI(g);
			ImGui::TreePop();
			ImGui::Separator();
		}
	}

	ImGui::End();
}