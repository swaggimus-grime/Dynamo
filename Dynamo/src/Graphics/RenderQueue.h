#pragma once

#include <queue>

class RenderQueue {
public:
	RenderQueue() = default;
	

private:
	std::queue<class Renderable> m_Queue;
};