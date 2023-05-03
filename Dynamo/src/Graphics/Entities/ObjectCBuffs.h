#pragma once

struct SpecularMaterial {
    XMFLOAT4 SpecColor;
	float SpecPower;
	int hasSpecMap;
	float padding[2];
};

struct CamStuff {
	XMFLOAT3 EyePos;
	float padding;
};