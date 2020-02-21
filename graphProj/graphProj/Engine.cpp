#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height, float posX, float posY)
{
	//keyboardInput.EnableAutoRepeatChars();
	if (!this->render_window.Initialize(this, hInstance, window_title, window_class, width, height, posX, posY))
		return false;
	if (!gfx.Init(render_window.GetHWND(), width, height))
		return false;

	return true;
}

bool Engine::ProcessMessage()
{
	return this->render_window.ProcessMessages();
}

void Engine::Update()
{
	while (!keyboardInput.CharBufferIsEmpty())
	{
		unsigned char ch = keyboardInput.ReadChar();
	}
	while (!keyboardInput.KeyBufferIsEmpty())
	{
		KeyboardInputEvent event = keyboardInput.ReadKey();
		unsigned char key = event.GetKeyCode();;
	}
	while (!mouseInput.EventBufferIsEmpty())
	{
		MouseInputEvent event = mouseInput.ReadEvent();
		if (mouseInput.IsRightDown())
		{
			if (event.GetType() == MouseInputEvent::EventType::RAW_MOVE)
			{
				
				this->gfx.myCamera.AdjustRotation((float)event.GetPosY() * 0.01f, (float)event.GetPosX() * 0.01f, 0);
			}
		}
	}

	const float cameraSpeed = 0.02f;

	if (keyboardInput.KeyIsPressed('W'))
	{
		this->gfx.myCamera.AdjustPosition(this->gfx.myCamera.GetForwardVector() * cameraSpeed);
	}
	if (keyboardInput.KeyIsPressed('S'))
	{
		this->gfx.myCamera.AdjustPosition(this->gfx.myCamera.GetBackwardVector() * cameraSpeed);
	}
	if (keyboardInput.KeyIsPressed('A'))
	{
		this->gfx.myCamera.AdjustPosition(this->gfx.myCamera.GetLeftVector() * cameraSpeed);
	}
	if (keyboardInput.KeyIsPressed('D'))
	{
		this->gfx.myCamera.AdjustPosition(this->gfx.myCamera.GetRightVector() * cameraSpeed);
	}
	if (keyboardInput.KeyIsPressed(VK_SPACE))
	{
		this->gfx.myCamera.AdjustPosition(0.0f, cameraSpeed, 0.0f);
	}
	if (keyboardInput.KeyIsPressed('Z'))
	{
		this->gfx.myCamera.AdjustPosition(0.0f, -cameraSpeed, 0.0f);
	}
	
}

void Engine::RenderFrame()
{
	gfx.RenderFrame();
}
