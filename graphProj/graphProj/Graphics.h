#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "Camera.h"
#include "model.h"
#include "GameObject.h"
#include "SphereKatamaria.h"
#include "ObjectKatamaria.h"
#include "KatamariaScene.h"
#include "Timer.h"
#include <vector>
#include <WICTextureLoader.h>
#include "Light.h"

class Graphics
{
public: 

	bool Init(HWND hwnd, int width, int height);
	void RenderFrame();
	Camera myCamera;
	std::vector <Light*>  light;
	std::vector <Model*>  models;
	std::vector <GameObject*>  GameObjects;
	Model* AddModel(XMFLOAT3 pos, Model* parent, XMFLOAT3 color);
	GameObject* AddGameObject(XMFLOAT3 pos, GameObject* parent, const std::string & filePath, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture);
	GameObject* AddLight(XMFLOAT3 pos, GameObject* parent);
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
	
	
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> StarDeath;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Ogon;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Mettal;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Grass;



private:
	bool InitDirecrX(HWND hwnd);
	bool InitShaders();
	bool InitScene();
	
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr <IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> rtv;
	Microsoft::WRL::ComPtr <ID3D11Device> device;

	ConstantBuffer<CB_VS_vertexshader> constBuffer_vertex;
	ConstantBuffer<CB_PS_light> constBuffer_light;
	VertexShader vertexSh;
	PixelShader pixelSh;
	


	


	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;


	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	

	int windowWidth = 0;
	int windowHeight = 0;


	Timer fpsTimer;



};