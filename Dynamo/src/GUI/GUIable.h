#pragma once

class D3DGUIable {
public:
	virtual void ShowGUI(class Graphics& g) = 0;
};

class GUIable {
public:
	virtual void ShowGUI() = 0;
};