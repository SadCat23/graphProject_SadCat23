#include "Graphics.h"

bool Graphics::Init(HWND hwnd, int width, int height)
{
	this->windowWidth = width;
	this->windowHeight = height;
	
	if (!InitDirecrX(hwnd))
		return false;

	if (!InitShaders())
		return false;

	if (!InitScene())
		return false;

	return true;
}

void Graphics::RenderFrame()
{
	float bgcolor[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	
	this->context->ClearRenderTargetView(this->rtv.Get(), bgcolor);
	this->context->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	this->context->IASetInputLayout(this->vertexSh.GetInputLayout());
	this->context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	this->context->RSSetState(this->rasterizerState.Get());
	this->context->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
	this->context->VSSetShader(vertexSh.GetShader(), NULL, 0);
	this->context->PSSetShader(pixelSh.GetShader(), NULL, 0);

	
	UINT offset = 0;

	//UpdateConstantBuffer
	XMMATRIX world = XMMatrixIdentity();
	
	constBuffer.data.mat = world * myCamera.GetViewMatrix() * myCamera.GetProjectionMatrix();
	constBuffer.data.mat = DirectX::XMMatrixTranspose(constBuffer.data.mat);



	if (!constBuffer.ApplyChanges())
	{
		return;
	}
	
	
	this->context->VSSetConstantBuffers(0, 1, constBuffer.GetAddressOf());


	
	this->context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
	this->context->IASetIndexBuffer(indecesBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	this->context->DrawIndexed(indecesBuffer.BufferSize(),0,0);

	//TExt

	spriteBatch->Begin();
	
	spriteFont->DrawString(spriteBatch.get(), L"By SadCat", DirectX::XMFLOAT2(0, 0), DirectX::Colors::White, 0.0f, DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(1, 1));
	spriteBatch->End();

	this->swapChain->Present(1, NULL);
}

bool Graphics::InitDirecrX(HWND hwnd)
{
	std::vector<AdapterData> adapters = AdapterReader::GetAdapters();

	if (adapters.size() < 1)
	{
		ErrorLogger::Log("No IDXGI Adapters found.");
		return false;
	}

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferDesc.Width = this->windowWidth;
	scd.BufferDesc.Height = this->windowHeight;
	scd.BufferDesc.RefreshRate.Numerator = 60;
	scd.BufferDesc.RefreshRate.Denominator = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;

	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = hwnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr;
	hr = D3D11CreateDeviceAndSwapChain(adapters[0].pAdapter, //IDXGI Adapter
		D3D_DRIVER_TYPE_UNKNOWN,
		NULL, //FOR SOFTWARE DRIVER TYPE
		NULL, //FLAGS FOR RUNTIME LAYERS
		NULL, //FEATURE LEVELS ARRAY
		0, //# OF FEATURE LEVELS IN ARRAY
		D3D11_SDK_VERSION,
		&scd, //Swapchain description
		this->swapChain.GetAddressOf(), //Swapchain Address
		this->device.GetAddressOf(), //Device Address
		NULL, //Supported feature level
		this->context.GetAddressOf()); //Device Context Address

	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create device and swapchain.");
		return false;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	if (FAILED(hr)) //If error occurred
	{
		ErrorLogger::Log(hr, "GetBuffer Failed.");
		return false;
	}

	hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->rtv.GetAddressOf());
	if (FAILED(hr)) //If error occurred
	{
		ErrorLogger::Log(hr, "Failed to create render target view.");
		return false;
	}

	// Describe our Depth / Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	depthStencilDesc.Width = this->windowWidth;
	depthStencilDesc.Height = this->windowHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	hr = this->device->CreateTexture2D(&depthStencilDesc, NULL, this->depthStencilBuffer.GetAddressOf());
	if (FAILED(hr)) //If error occurred
	{
		ErrorLogger::Log(hr, "Failed to create depth stencil buffer.");
		return false;
	}

	hr = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());
	if (FAILED(hr)) //If error occurred
	{
		ErrorLogger::Log(hr, "Failed to create depth stencil view.");
		return false;
	}








	this->context->OMSetRenderTargets(1, this->rtv.GetAddressOf(), this->depthStencilView.Get());

	//Create depth stencil state
	D3D11_DEPTH_STENCIL_DESC depthstencildesc;
	ZeroMemory(&depthstencildesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

	depthstencildesc.DepthEnable = true;
	depthstencildesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthstencildesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

	hr = this->device->CreateDepthStencilState(&depthstencildesc, this->depthStencilState.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create depth stencil state.");
		return false;
	}

	//Create the Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = this->windowWidth;
	viewport.Height = this->windowHeight;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;
	//Set the Viewport
	this->context->RSSetViewports(1, &viewport);

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	hr = this->device->CreateRasterizerState(&rasterizerDesc, this->rasterizerState.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create rasterizer state.");
		return false;
	}
	spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->context.Get());
	spriteFont  = std::make_unique<DirectX::SpriteFont>(this->device.Get(),L"Data\\Fonts\\comic_sans_ms_16.spritefont");




	return true;
}

bool Graphics::InitShaders()
{

	std::wstring shaderfolder = L"";
#pragma region DetermineShaderPath
	if (IsDebuggerPresent() == TRUE)
	{
#ifdef _DEBUG //Debug Mode
#ifdef _WIN64 //x64
		shaderfolder = L"..\\x64\\Debug\\";
#else  //x86 (Win32)
		shaderfolder = L"..\\Debug\\";
#endif
#else //Release Mode
#ifdef _WIN64 //x64
		shaderfolder = L"..\\x64\\Release\\";
#else  //x86 (Win32)
		shaderfolder = L"..\\Release\\";
#endif
#endif
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
		{"COLOR", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0,D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0  },
	};

	UINT numElements = ARRAYSIZE(layout);

	if (!vertexSh.Init(this->device, shaderfolder + L"vertexshader.cso", layout, numElements))
		return false;

	if (!pixelSh.Init(this->device, shaderfolder + L"pixelshader.cso"))
		return false;


	return true;
}

bool Graphics::InitScene()
{

	Vertex v[] =
	{
		Vertex(-0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 1.0f), //Bottom Left   - [0]
		Vertex(-0.5f,   0.5f, 0.0f, 1.0f, 0.0f, 0.0f), //Top Left      - [1]
		Vertex(0.5f,   0.5f, 0.0f, 1.0f, 1.0f, 0.0f), //Top Right     - [2]
		Vertex(0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, 1.0f), //Bottom Right   - [3]
	};

	HRESULT hr = this->vertexBuffer.Initialize(this->device.Get(), v, ARRAYSIZE(v));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create vertex buffer.");
		return false;
	}


	DWORD indeces[] =
	{
		0,1,2,
		0,2,3
	};


	hr = this->indecesBuffer.Initialize(this->device.Get(), indeces, ARRAYSIZE(indeces));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create indices buffer.");
		return hr;
	}


	hr = this->constBuffer.Initialize(this->device.Get(), this->context.Get());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to initialize constant buffer.");
		return false;
	}



	myCamera.SetPosition(0.0f, 0.0f, -2.0f);
	myCamera.SetProjectionValues(60.0f, static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 1000.0f);
	return true;
}