#pragma once
#include <DirectXMath.h>

struct CB_VS_vertexshader
{
	DirectX::XMMATRIX wvpMatrix;
	DirectX::XMMATRIX worldMatrix;
}; 
struct CB_PS_light
{
	DirectX::XMFLOAT3 ambientLoghtColor;
	float ambientStrength;
	
	DirectX::XMFLOAT3 dynamicLightColor;
	float dynamicStrength;
	DirectX::XMFLOAT3 dynamicLightPosition;
	float dynamicAttenuation_a;
	float dynamicAttenuation_b;
	float dynamicAttenuation_c;
};