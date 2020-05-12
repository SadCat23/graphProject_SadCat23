#pragma once
#include "WindowContainer.h"
#include "Timer.h"

class Engine : WindowContainer
{

public:
	float deltaTime;
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height, float posX, float posY);
	bool ProcessMessage();
	Graphics* GetGraphicsEngine();
	void Update();
	void RenderFrame();
	Model* AddModel(XMFLOAT3 pos, Model* parent, XMFLOAT3 color);
	GameObject* AddLight(XMFLOAT3 pos, GameObject* parent);
	GameObject* AddGameObject(XMFLOAT3 pos, GameObject* parent, const std::string & filePath, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture);
private:
	Timer timer;
};
