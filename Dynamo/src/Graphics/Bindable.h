#pragma once

class Bindable {
public:
	virtual void Bind(class Graphics& g) const = 0;
};