#include "dynamopch.h"
#include "VertexBuffer.h"

VertexLayout::VertexLayout(ATTRIB_FLAGS flags)
	:m_Stride(0), m_Flags(flags)
{
	if (m_Flags & ATTRIB_POS) {
		m_Stride += sizeof(XMFLOAT3);
		m_Elements.push_back({ "Pos", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	}
	if (m_Flags & ATTRIB_TEX) {
		m_Stride += sizeof(XMFLOAT2);
		m_Elements.push_back({ "Tex", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	}
	if (m_Flags & ATTRIB_NORM) {
		m_Stride += sizeof(XMFLOAT3);
		m_Elements.push_back({ "Norm", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	}
	if (m_Flags & ATTRIB_TAN) {
		m_Stride += sizeof(XMFLOAT3);
		m_Elements.push_back({ "Tan", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	}
	if (m_Flags & ATTRIB_BITAN) {
		m_Stride += sizeof(XMFLOAT3);
		m_Elements.push_back({ "Bitan", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	}
	if (m_Flags & ATTRIB_BONES) {
		m_Stride += sizeof(XMINT4);
		m_Elements.push_back({ "Bones", 0, DXGI_FORMAT_R32G32B32A32_SINT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0});
	}
	if (m_Flags & ATTRIB_WEIGHTS) {
		m_Stride += sizeof(XMFLOAT4);
		m_Elements.push_back({ "Weights", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 });
	}
}

VertexData::VertexData(VertexLayout layout, SIZE_T size)
	:m_Layout(layout), m_Size(size)
{
	if (m_Layout.Flags() & ATTRIB_TEX)
		m_TexSize += sizeof(XMFLOAT2);
	if (m_Layout.Flags() & ATTRIB_NORM)
		m_NormSize += sizeof(XMFLOAT3);
	if (m_Layout.Flags() & ATTRIB_TAN)
		m_TanSize += sizeof(XMFLOAT3);
	if (m_Layout.Flags() & ATTRIB_BITAN)
		m_BitanSize += sizeof(XMFLOAT3);
	if (m_Layout.Flags() & ATTRIB_BONES)
		m_BoneSize += sizeof(XMINT4);
	if (m_Layout.Flags() & ATTRIB_WEIGHTS)
		m_WeightSize += sizeof(XMFLOAT4);

	m_Bytes.resize(size * m_Layout.Stride());
}

XMFLOAT3& VertexData::Pos(UINT vertIdx)
{
	DYNAMO_ASSERT(m_Layout.Flags() & ATTRIB_POS && vertIdx >= 0 && vertIdx < m_Size,
		"Invalid index");

	return *reinterpret_cast<XMFLOAT3*>(&m_Bytes[vertIdx * m_Layout.Stride()]);
}

XMFLOAT2& VertexData::Tex(UINT vertIdx)
{
	DYNAMO_ASSERT(m_Layout.Flags() & ATTRIB_TEX && vertIdx >= 0 && vertIdx < m_Size,
		"Invalid index");

	return *reinterpret_cast<XMFLOAT2*>(&m_Bytes[vertIdx * m_Layout.Stride() + m_PosSize]);
}

XMFLOAT3& VertexData::Norm(UINT vertIdx)
{
	DYNAMO_ASSERT(m_Layout.Flags() & ATTRIB_NORM && vertIdx >= 0 && vertIdx < m_Size,
		"Invalid index");

	return *reinterpret_cast<XMFLOAT3*>(&m_Bytes[vertIdx * m_Layout.Stride() + m_PosSize + m_TexSize]);
}

XMFLOAT3& VertexData::Tan(UINT vertIdx)
{
	DYNAMO_ASSERT(m_Layout.Flags() & ATTRIB_TAN && vertIdx >= 0 && vertIdx < m_Size,
		"Invalid index");

	return *reinterpret_cast<XMFLOAT3*>(&m_Bytes[vertIdx * m_Layout.Stride() + m_PosSize + m_TexSize + m_NormSize]);
}

XMFLOAT3& VertexData::Bitan(UINT vertIdx)
{
	DYNAMO_ASSERT(m_Layout.Flags() & ATTRIB_BITAN && vertIdx >= 0 && vertIdx < m_Size,
		"Invalid index");

	return *reinterpret_cast<XMFLOAT3*>(&m_Bytes[vertIdx * m_Layout.Stride() + m_PosSize + m_TexSize + m_NormSize + m_TanSize]);
}

XMINT4& VertexData::Bones(UINT vertIdx)
{
	DYNAMO_ASSERT(m_Layout.Flags() & ATTRIB_BONES && vertIdx >= 0 && vertIdx < m_Size,
		"Invalid index");

	return *reinterpret_cast<XMINT4*>(&m_Bytes[vertIdx * m_Layout.Stride() + m_PosSize + m_TexSize + m_NormSize + m_TanSize + m_BitanSize]);
}

XMFLOAT4& VertexData::Weights(UINT vertIdx)
{
	DYNAMO_ASSERT(m_Layout.Flags() & ATTRIB_WEIGHTS && vertIdx >= 0 && vertIdx < m_Size,
		"Invalid index");

	return *reinterpret_cast<XMFLOAT4*>(&m_Bytes[vertIdx * m_Layout.Stride() + m_PosSize + m_TexSize + m_NormSize + m_TanSize + m_BitanSize + m_BoneSize]);
}

VertexBuffer::VertexBuffer(Graphics& g, const VertexData& vertData)
	:VertexBuffer(g, "NULL", vertData)
{
}

VertexBuffer::VertexBuffer(Graphics& g, const std::string& id, const VertexData& vertData)
	:m_Stride(vertData.Layout().Stride()), m_ID(std::move(id))
{
	D3D11_BUFFER_DESC buffDesc = { 0 };
	buffDesc.Usage = D3D11_USAGE_DEFAULT;
	buffDesc.ByteWidth = vertData.Size();
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	buffDesc.CPUAccessFlags = NULL;
	buffDesc.MiscFlags = NULL;
	buffDesc.StructureByteStride = m_Stride;

	D3D11_SUBRESOURCE_DATA subResData{};
	subResData.pSysMem = vertData.Data();
	subResData.SysMemPitch = 0;
	subResData.SysMemSlicePitch = 0;

	g.Device().CreateBuffer(&buffDesc, &subResData, &m_Buff);
}

template<typename...Ignore>
std::string VertexBuffer::CreateHash(const std::string& id, Ignore&&...ignore)
{
	return typeid(VertexBuffer).name() + "#"s + id;
}

void VertexBuffer::Bind(Graphics& g)
{
	static const UINT offset = 0;
	g.DC().IASetVertexBuffers(0, 1, m_Buff.GetAddressOf(), &m_Stride, &offset);
}

Shared<VertexBuffer> VertexBuffer::Evaluate(Graphics& g, const std::string& id, const VertexData& vertData)
{
	return Binds::Evaluate<VertexBuffer>(g, id, vertData);
}
