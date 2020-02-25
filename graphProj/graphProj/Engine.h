#pragma once
#include "WindowContainer.h"
#include "Timer.h"
class Engine : WindowContainer
{

public:
	bool Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height, float posX, float posY);
	bool ProcessMessage();
	void Update();
	void RenderFrame();
	Model* AddModel(XMFLOAT3 pos, Model* parent, XMFLOAT3 color);
private:
	Timer timer;
};
