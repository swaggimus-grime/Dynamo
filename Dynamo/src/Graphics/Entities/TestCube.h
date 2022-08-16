#pragma once

#include "Render/Renderable.h"
#include "Bindable/TransformBuffer.h"

class TestCube : public Renderable, public Transformable
{
public:
	TestCube(Graphics& g, float size);
	virtual XMMATRIX ModelMat() const override;
	virtual void ShowGUI() override;
};