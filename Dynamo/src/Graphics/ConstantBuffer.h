#pragma once

#include "Bindable.h"

class ConstantBuffer {
protected:
	ConstantBuffer() = delete;
	ConstantBuffer(class Graphics& g, SIZE_T size, UINT slot, LPVOID data);
	void Update(Graphics& g, SIZE_T size, LPVOID data);
protected:
	ComPtr<ID3D11Buffer> m_Buff;
	UINT m_Slot;
};

template<class T>
class VertexConstantBuffer : public ConstantBuffer, public Bindable {
public:
	VertexConstantBuffer(Graphics& g, UINT slot = 0, LPVOID data = nullptr)
		:ConstantBuffer(g, sizeof(T), slot, data), Bindable(CreateHash(++m_Instance))
	{
	}

	void Update(Graphics& g, LPVOID data)
	{
		__super::Update(g, sizeof(T), data);
	}

	virtual void Bind(Graphics& g) override
	{
		g.DC().VSSetConstantBuffers(m_Slot, 1, m_Buff.GetAddressOf());
	}

	static Shared<VertexConstantBuffer<T>> Evaluate(Graphics& g, UINT slot = 0, LPVOID data = nullptr)
	{
		return Binds::Evaluate<VertexConstantBuffer<T>>(g, slot, data);
	}

	static std::string CreateHash(UINT instance)
	{
		return typeid(VertexConstantBuffer<T>).name() + std::to_string(instance);
	}
	
private:
	static inline UINT m_Instance = 0;
};

template<class T>
class PixelConstantBuffer : public ConstantBuffer, public Bindable {
public:
	PixelConstantBuffer(Graphics& g, UINT slot = 0, LPVOID data = nullptr)
		:ConstantBuffer(g, sizeof(T), slot, data), Bindable(CreateHash(++m_Instance))
	{
	}

	void Update(Graphics& g, LPVOID data)
	{
		__super::Update(g, sizeof(T), data);
	}

	virtual void Bind(Graphics& g) override
	{
		g.DC().PSSetConstantBuffers(m_Slot, 1, m_Buff.GetAddressOf());
	}

	static Shared<PixelConstantBuffer<T>> Evaluate(Graphics& g, UINT slot = 0, LPVOID data = nullptr)
	{
		return Binds::Evaluate<PixelConstantBuffer<T>>(g, slot, data);
	}

	static std::string CreateHash(UINT instance)
	{
		return typeid(PixelConstantBuffer<T>).name() + std::to_string(instance);
	}
private:
	static inline UINT m_Instance = 0;
};

