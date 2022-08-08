#pragma once

#include "Bindable.h"
#include <d3d11.h>

class IndexBuffer : public Bindable {
public:
	IndexBuffer(Graphics& g, const std::vector<UINT>& indices);
	IndexBuffer(Graphics& g, const std::string& id, const std::vector<UINT>& indices);
	inline UINT Count() const { return m_Count; }
	virtual void Bind(Graphics& g) override;

	template<typename...Ignore>
	static std::string CreateHash(const std::string& id, Ignore&&...ignore);
	static Shared<IndexBuffer> Evaluate(Graphics& g, const std::string& id, const std::vector<UINT>& indices);

private:
	ComPtr<ID3D11Buffer> m_Buff;
	UINT m_Count;
	std::string m_ID;
};