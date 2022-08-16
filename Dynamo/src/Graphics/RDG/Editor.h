#pragma once

class Editor {
public:
	Editor() = delete;
	static UINT NextNode();
	static UINT NextPin();
	static UINT NextLink();

private:
	static UINT s_Node;
	static UINT s_Pin;
	static UINT s_Link;
};