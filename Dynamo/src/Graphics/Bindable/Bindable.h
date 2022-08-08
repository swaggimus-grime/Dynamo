#pragma once

class Bindable {
public:
	virtual void Bind(class Graphics& g) = 0;
	virtual void SetParent(const class Renderable& parent)
	{
	}
};