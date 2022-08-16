#include "Editor.h"
#include "dynamopch.h"
#include "Editor.h"

UINT Editor::s_Node = 0x1;
UINT Editor::s_Pin = 0xFF;
UINT Editor::s_Link = 0xFFFF;

UINT Editor::NextNode()
{
	return s_Node++;
}

UINT Editor::NextPin()
{
	return s_Node++;
}

UINT Editor::NextLink()
{
	return s_Node++;
}
