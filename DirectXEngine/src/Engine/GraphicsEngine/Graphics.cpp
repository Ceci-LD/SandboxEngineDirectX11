#include "Graphics.h"
#include <iostream>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"
#include "Shaders/VertexShader.h"
#include "Shaders/PixelShader.h"
#include "InputLayout.h"
#include "Topology.h"

//#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;

Graphics::Graphics(HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC scDesc = {};
	scDesc.BufferDesc.Width = 0;
	scDesc.BufferDesc.Height = 0;
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.BufferDesc.RefreshRate.Numerator = 0;
	scDesc.BufferDesc.RefreshRate.Denominator = 0;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.SampleDesc.Count = 1;
	scDesc.SampleDesc.Quality = 0;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = 1;
	scDesc.OutputWindow = hwnd;
	scDesc.Windowed = TRUE;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scDesc.Flags = 0;

	//Create device front and back buffers;
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&scDesc,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	);

	if (FAILED(hr))
	{
		std::cerr << "D3D11CreateDeviceAndSwapChain Failed : " << hr << std::endl;
	}

	//Back Buffer
	wrl::ComPtr<ID3D11Resource> pBackBuffer = nullptr;
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	hr = pDevice->CreateRenderTargetView(
		pBackBuffer.Get(),
		nullptr,
		&pTarget
	);

	//Configure VP
	D3D11_VIEWPORT vp;
	vp.Width = 1280;
	vp.Height = 720;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pContext->RSSetViewports(1u, &vp);
}

Graphics::~Graphics()
{
}

void Graphics::ClearBuffer(float r, float g, float b)
{
	const float color[] = { r, g, b, 1.0f };
	pContext->ClearRenderTargetView(pTarget.Get(), color);
}

void Graphics::EndFrame()
{
	pSwap->Present(1u, 0u);
}

void Graphics::DrawTriangle(float angle, float x, float y)
{
	HRESULT hr;

	struct Vertex
	{
		float x, y;
		unsigned char r, g, b;
	};

	const std::vector<Vertex> vertices =
	{
		{  0.0f,  0.5f,  255, 0, 0 },
		{  0.5f, -0.5f,  0, 255, 0 },
		{ -0.5f, -0.5f,  0, 0, 255 },
	};

	const std::vector<unsigned short> indices =
	{
		0, 1, 2
	};

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	//Create VB
	auto vb = std::make_unique<VertexBuffer>(*this, vertices);
	vb->Bind(*this);

	//Create IB
	std::unique_ptr<IndexBuffer> ib = std::make_unique<IndexBuffer>(*this, indices);
	ib->Bind(*this);

	//Create Constant Buffer
	struct ConstantBuffer
	{
		dx::XMMATRIX transform;
	};

	const ConstantBuffer cb =
	{
		dx::XMMatrixTranspose(
			dx::XMMatrixRotationZ(angle) *
			dx::XMMatrixScaling(9.0f / 16.0f, 1.0f, 1.0f) *
			dx::XMMatrixTranslation(x, y, 0.0f)
		)
	};

	wrl::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	hr = pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer);
	if (FAILED(hr))
	{
		std::cerr << "CreateBuffer failed : " << hr << std::endl;
	}

	//Bind ConstantBuffer
	pContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());






	//Create VS
	auto vs = std::make_unique<VertexShader>(*this, L"VertexShader.cso");
	vs->Bind(*this);

	//Create PS
	auto ps = std::make_unique<PixelShader>(*this, L"PixelShader.cso");
	ps->Bind(*this);

	//Create Input Layout
	auto layout = std::make_unique<InputLayout>(*this, ied, vs->GetBlob());
	layout->Bind(*this);

	//Bind RenderTarget
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	//Primiive topology
	auto topology = std::make_unique<Topology>(*this, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	topology->Bind(*this);

	//Draw
	pContext->DrawIndexed((UINT)indices.size(), 0u, 0u);
}
