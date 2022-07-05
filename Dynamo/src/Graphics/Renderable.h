#pragma once

#include "GUI/GUIable.h"

class Renderable : public GUIable {
public:
	virtual void Render(class Graphics& g) = 0;
	
};