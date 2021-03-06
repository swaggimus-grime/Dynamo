#pragma once

#include "Buffer.h"
#include "Texture.h"
#include "Renderable.h"
#include "Sampler.h"
#include "imgui.h"

template<class T>
class Mesh : public Renderable {
public:
	Mesh(Graphics& g, const std::vector<T>& vertices, ID3D10Blob& vsCode, VertexLayout& layout, const std::vector<UINT>& indices)
	{
		m_VBuff = std::make_shared<VertexBuffer<T>>(g, std::move(vertices), vsCode, layout);
		m_IBuff = std::make_shared<IndexBuffer>(g, std::move(indices));
	}

	inline void AddTexture(std::shared_ptr<Texture2D> tex) { m_Textures.push_back(std::move(tex)); }

	virtual void Render(Graphics& g) override
	{
		m_VBuff->Bind(g);
		m_IBuff->Bind(g);
		for (const auto& t : m_Textures)
			t->Bind(g);

		g.DC().DrawIndexed(m_IBuff->Size(), 0, 0);
	}

	virtual void ShowGUI(Graphics& g) override
	{
		ImGui::Text("Vertices: ", m_VBuff->Size());
		ImGui::Text("Indices: ", m_IBuff->Size());
	}

	inline UINT GetIndexCount() const { return m_IBuff->Size(); }
	inline std::shared_ptr<Texture2D>& GetTexture(UINT slot) { return m_Textures[slot]; }
private:
	std::shared_ptr<VertexBuffer<T>> m_VBuff;
	std::shared_ptr<IndexBuffer> m_IBuff;
	std::vector<std::shared_ptr<Texture2D>> m_Textures;
};