#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include "Vertex.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Camera.h"

class Graphics
{
public: 
	bool Init(HWND hwnd, int width, int height);
	void RenderFrame();
	Camera myCamera;
private:
	bool InitDirecrX(HWND hwnd);
	bool InitShaders();
	bool InitScene();
	
	Microsoft::WRL::ComPtr <ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr <IDXGISwapChain> swapChain;
	Microsoft::WRL::ComPtr <ID3D11RenderTargetView> rtv;
	Microsoft::WRL::ComPtr <ID3D11Device> device;

	ConstantBuffer<CB_VS_vertexshader> constBuffer;
	VertexShader vertexSh;
	PixelShader pixelSh;
	


	VertexBuffer<Vertex> vertexBuffer;
	IndexBuffer indecesBuffer;


	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;


	Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;
	

	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::unique_ptr<DirectX::SpriteFont> spriteFont;

	

	int windowWidth = 0;
	int windowHeight = 0;



};