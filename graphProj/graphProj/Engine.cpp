#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height, float posX, float posY)
{
	timer.Start();
	
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
	float dt = timer.GetMilisecondsElapsed();
	timer.Restart();
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


	//planet1
	gfx.models[1]->AdjustRotation(0.0f, 0.001f*dt, 0.0f);
	gfx.models[1]->AdjustRelativeRotation(0.0f, 0.0001f*dt, 0.0f);

	gfx.models[2]->AdjustRotation(0.0f, 0.0f, 0.001f*dt);
	gfx.models[2]->AdjustRelativeRotation(0.0f, 0.0f, 0.001f*dt);
	
	gfx.models[3]->AdjustRotation(0.0f, 0.001f*dt, 0.0f);
	gfx.models[3]->AdjustRelativeRotation(0.0f, 0.003f*dt, 0.0f);

	////////////////////////////////////////////////////

	gfx.models[4]->AdjustRotation(0.0f, 0.001f*dt, 0.0f);
	gfx.models[4]->AdjustRelativeRotation(0.0f, 0.0002f*dt, 0.0f);

	gfx.models[5]->AdjustRotation(0.0f, 0.0f, 0.001f*dt);
	gfx.models[5]->AdjustRelativeRotation(0.0f, 0.003f*dt, 0.0f );
	   	 	
	//////////////////////////
	gfx.models[6]->AdjustRotation(0.0f, 0.0003f*dt, 0.0f);
	gfx.models[6]->AdjustRelativeRotation(0.0f, 0.00005f*dt, 0.0f);

	gfx.models[7]->AdjustRotation(0.0f, 0.0f, 0.0001f*dt);
	gfx.models[7]->AdjustRelativeRotation(0.0f, 0.0f, 0.0003f*dt);


	gfx.models[8]->AdjustRotation(0.0f, 0.0f, 0.0001f*dt);
	gfx.models[8]->AdjustRelativeRotation(0.0f, 0.007f*dt,0.0f );



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

Model* Engine::AddModel(XMFLOAT3 pos, Model* parent, XMFLOAT3 color)
{
	return gfx.AddModel(pos, parent, color);
	
}
