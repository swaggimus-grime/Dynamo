#pragma once

#include "Bindable.h"

class IndexBuffer : public Bindable {
public:
	IndexBuffer(Graphics& g, const std::vector<UINT>& indices);
	virtual void Bind(Graphics& g) override;
	static std::string CreateHash(const std::vector<UINT>& indices);
	inline UINT Count() const { return m_Count; }
private:
	ComPtr<ID3D11Buffer> m_Buff;
	UINT m_Count;
};