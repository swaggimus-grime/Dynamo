#pragma once

#include "Buffer.h"
#include "Texture.h"
#include "Renderable.h"

template<class T>
class Mesh : public Renderable {
public:
	Mesh(std::shared_ptr<VertexBuffer<T>> vbuff, std::shared_ptr<IndexBuffer> ibuff)
		:m_VBuff(std::move(vbuff)), m_IBuff(std::move(ibuff))
	{
	}

	inline void AddTexture(std::shared_ptr<Texture2D> tex) { m_Textures.push_back(std::move(tex)); }

	virtual void Render(Graphics& g) const override
	{
		m_VBuff->Bind(g);
		m_IBuff->Bind(g);
		for (const auto& t : m_Textures)
			t->Bind(g);

		g.DC().DrawIndexed(m_IBuff->Size(), 0, 0);
	}

	inline UINT GetIndexCount() const { return m_IBuff->Size(); }
private:
	std::shared_ptr<VertexBuffer<T>> m_VBuff;
	std::shared_ptr<IndexBuffer> m_IBuff;
	std::vector<std::shared_ptr<Texture2D>> m_Textures;
};