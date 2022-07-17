#pragma once

#include "Bindable.h"

enum ATTRIB_FLAGS {
	ATTRIB_POS = 0x1,
	ATTRIB_TEX = 0x10,
	ATTRIB_NORM = 0x100,
	ATTRIB_TAN = 0x1000,
	ATTRIB_BITAN = 0x10000
};

inline ATTRIB_FLAGS operator|(ATTRIB_FLAGS a, ATTRIB_FLAGS b)
{
	return static_cast<ATTRIB_FLAGS>(static_cast<int>(a) | static_cast<int>(b));
}

class VertexLayout {
public:
	VertexLayout(ATTRIB_FLAGS flags);
	const D3D11_INPUT_ELEMENT_DESC* Data() const { return m_Elements.data(); }
	const SIZE_T Size() const { return m_Elements.size(); }
	inline SIZE_T Stride() const { return m_Stride; }
	inline ATTRIB_FLAGS Flags() const { return m_Flags; }
private:
	std::vector<D3D11_INPUT_ELEMENT_DESC> m_Elements;
	SIZE_T m_Stride;
	ATTRIB_FLAGS m_Flags;
};

class VertexData {
public:
	VertexData(VertexLayout layout, SIZE_T size);
	const CHAR* Data() const { return m_Bytes.data(); }
	SIZE_T Size() const { return m_Bytes.size(); }
	const VertexLayout& Layout() const { return m_Layout; }

	XMFLOAT3& Pos(UINT vertIdx);
	XMFLOAT2& Tex(UINT vertIdx);
	XMFLOAT3& Norm(UINT vertIdx);
	XMFLOAT3& Tan(UINT vertIdx);
	XMFLOAT3& Bitan(UINT vertIdx);

private:
	SIZE_T m_Size;
	std::vector<CHAR> m_Bytes;
	VertexLayout m_Layout;

	SIZE_T m_PosSize = sizeof(XMFLOAT3);
	SIZE_T m_TexSize = 0;
	SIZE_T m_NormSize = 0;
	SIZE_T m_TanSize = 0;
	SIZE_T m_BitanSize = 0;
};

class VertexBuffer : public Bindable {
public:
	VertexBuffer(Graphics& g, const VertexData& vertData);
	static std::string CreateHash(const VertexData& vertData);
	virtual void Bind(Graphics& g) override;
private:
	ComPtr<ID3D11Buffer> m_Buff;
	UINT m_Stride;
};