//#pragma once
//
//#include "Bindable.h"
//
//enum class BLEND_MODE {
//	NONE, DEFAULT
//};
//
//class BlendState : public Bindable {
//public:
//	BlendState(Graphics& g, BLEND_MODE mode = BLEND_MODE::DEFAULT, const std::optional<float>& blendFactor = {});
//	virtual void Bind(Graphics& g) override;
//private:
//	ComPtr<ID3D11BlendState> m_State;
//	std::optional<float> m_Factor;
//};