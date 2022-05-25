#pragma once

class Renderable {
protected:
	virtual void Render(class Graphics& g) = 0;
};