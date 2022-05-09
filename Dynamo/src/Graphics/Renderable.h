#pragma once

class Renderable {
protected:
	friend class Graphics;

	virtual void Render() const = 0;
	virtual class Shader& GetShader() const = 0;
};