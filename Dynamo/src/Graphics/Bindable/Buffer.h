#pragma once

#include "Bindable.h"

class Buffer : public Bindable
{
public:
	virtual ~Buffer() = default;
	virtual void BindBuffer(Graphics& g) = 0;
	virtual void BindBuffer(Graphics& g, Buffer* other) = 0;
	virtual void Clear(Graphics&) = 0;
};