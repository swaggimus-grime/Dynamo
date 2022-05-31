#pragma once

class Renderable {
public:
	virtual void RenderOutline(class Graphics& g) = 0;
	virtual void Render(class Graphics& g) = 0;
};