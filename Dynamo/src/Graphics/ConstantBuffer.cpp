#include "ConstantBuffer.h"
#include "ConstantBuffer.h"
#include "ConstantBuffer.h"
#include "ConstantBuffer.h"
#include "ConstantBuffer.h"
#include "dynamopch.h"
#include "ConstantBuffer.h"

#include "Graphics.h"
#include "Binds.h"

ConstantBuffer::ConstantBuffer(Graphics& g, SIZE_T size, UINT slot, LPVOID data)
	:m_Slot(slot)
{
	D3D11_BUFFER_DESC desc = {};
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0u;
	desc.ByteWidth = size;
	desc.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA resData = {};
	if (data) 
		resData.pSysMem = data;

	g.Device().CreateBuffer(&desc, data ? &resData : nullptr, &m_Buff);
}

void ConstantBuffer::Update(Graphics& g, SIZE_T size, LPVOID data)
{
	D3D11_MAPPED_SUBRESOURCE res;
	g.DC().Map(m_Buff.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
	memcpy(res.pData, data, size);
	g.DC().Unmap(m_Buff.Get(), 0);
}
