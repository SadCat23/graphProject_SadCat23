#pragma once
#include "GameObject.h"

class Light : public GameObject
{
public:
	bool Initialize(ID3D11Device * device, ID3D11DeviceContext * deviceContext,ID3D11ShaderResourceView * texture,ConstantBuffer<CB_VS_vertexshader> & cb_vs_vertexshader, ConstantBuffer<CB_PS_light> & cb_ps_lightBuffer);
	DirectX::XMFLOAT3 lightColor = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	float lightStrength = 1.0f;
	float attenuation_a = 1.0f;
	float attenuation_b = 0.1f;
	float attenuation_c = 0.1f;
	
	//shadow build
	int ShadowMapSize = 4096;
	ID3D11DepthStencilView* DepthStancilVievwLight;
	ID3D11Texture2D* resource;
	ID3D11ShaderResourceView* DepthShaderViewLight;
	ID3D11RasterizerState* rastState;

	ID3D11DeviceContext * deviceContext;
	ConstantBuffer<CB_PS_light> * constBuffer_light;
	Light() {};
	~Light() {}
	typeObject GetType() override
	{
		return light;
	}
	
	void RenderLight();
};