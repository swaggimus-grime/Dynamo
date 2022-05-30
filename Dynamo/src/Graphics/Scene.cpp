#include "Scene.h"
#include "dynamopch.h"
#include "Scene.h"
#include <imgui.h>

Scene::Scene()
{

}

void Scene::Submit(std::shared_ptr<GUIable> g, const std::string& name)
{
	auto& it = m_GUIables.find(name);
	if (it != m_GUIables.end())
		it->second = g;
	else
		m_GUIables.insert({ name, g });
}

void Scene::Submit(std::shared_ptr<D3DGUIable> g, const std::string& name)
{
	auto& it = m_D3DGUIables.find(name);
	if (it != m_D3DGUIables.end())
		it->second = g;
	else
		m_D3DGUIables.insert({ name, g });
}

void Scene::ShowGUI(Graphics& g)
{
	ImGui::Begin("Scene");
	for (const auto& d3dg : m_D3DGUIables) {
		if (ImGui::TreeNode(d3dg.first.c_str()))
		{
			d3dg.second->ShowGUI(g);
			ImGui::TreePop();
			ImGui::Separator();
		}
	}

	ShowGUI();
	ImGui::End();
}

void Scene::ShowGUI()
{
	for (const auto& g : m_GUIables) {
		if (ImGui::TreeNode(g.first.c_str()))
		{
			g.second->ShowGUI();
			ImGui::TreePop();
			ImGui::Separator();
		}
	}
}