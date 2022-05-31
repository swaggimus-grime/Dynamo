#pragma once

#include "Buffer.h"
#include "Texture.h"
#include "Renderable.h"
#include "Sampler.h"

template<class T>
class Mesh : public Renderable {
public:
	Mesh(Graphics& g, const std::vector<T>& vertices, ID3D10Blob& vsCode, VertexLayout& layout, const std::vector<UINT>& indices)
	{
		m_VBuff = std::make_shared<VertexBuffer<T>>(g, vertices, vsCode, layout);
		m_IBuff = std::make_shared<IndexBuffer>(g, indices);
		m_Sampler = std::make_shared<Sampler>(g, SAMPLER_MODE::LINEAR_CLAMP);
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

	virtual void RenderOutline(Graphics& g) override
	{
		m_VBuff->Bind(g);
		m_IBuff->Bind(g);
		g.DC().DrawIndexed(m_IBuff->Size(), 0, 0);
	}

	inline UINT GetIndexCount() const { return m_IBuff->Size(); }
	inline std::shared_ptr<Texture2D>& GetTexture(UINT slot) { return m_Textures[slot]; }
private:
	std::shared_ptr<VertexBuffer<T>> m_VBuff;
	std::shared_ptr<IndexBuffer> m_IBuff;
	std::vector<std::shared_ptr<Texture2D>> m_Textures;
	std::shared_ptr<Sampler> m_Sampler;
};