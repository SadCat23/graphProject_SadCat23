#include "Engine.h"
#include"KatamariaGame.h"
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

Graphics* Engine::GetGraphicsEngine()
{
	return &gfx;
}

void Engine::Update()
{
	this->deltaTime = timer.GetMilisecondsElapsed();
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
	


	KatamariaGame::getInstance().GameLoop();

	const float cameraSpeed = 0.02f;

	if (keyboardInput.KeyIsPressed('W'))
	{	
		KatamariaGame::getInstance().Input('W');
	}
	if (keyboardInput.KeyIsPressed('S'))
	{
		KatamariaGame::getInstance().Input('S');

	}
	if (keyboardInput.KeyIsPressed('A'))
	{
		KatamariaGame::getInstance().Input('A');
	}
	if (keyboardInput.KeyIsPressed('D'))
	{
		KatamariaGame::getInstance().Input('D');
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

GameObject * Engine::AddLight(XMFLOAT3 pos, GameObject * parent)
{
	return gfx.AddLight(pos, parent);
	return nullptr;
}

GameObject* Engine::AddGameObject(XMFLOAT3 pos, GameObject* parent, const std::string & filePath, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture)
{

	return gfx.AddGameObject(pos, parent, filePath,texture);

}