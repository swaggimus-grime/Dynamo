#pragma once

#include "RDG.h"

class OutlineRDG : public RDG {
public:
	OutlineRDG(Graphics& g);
	void SetCamera(class Camera& camera);
	void SetLightCamera(Camera& camera);
};