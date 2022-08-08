#include "dynamopch.h"
#include "Out.h"

#include "Bindable/Bindable.h"
#include "Bindable/Buffer.h"

void Out::ValidateLink() const
{
}

Out::Out(const std::string& name)
	:Channel(name)
{
}