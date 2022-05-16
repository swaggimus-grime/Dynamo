#pragma once

class Renderable {
protected:
	virtual void Render(Graphics& g) const = 0;
};