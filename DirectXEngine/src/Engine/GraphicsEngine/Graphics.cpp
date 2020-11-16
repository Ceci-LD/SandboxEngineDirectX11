#include "Graphics.h"
#include <iostream>
#include <d3dcompiler.h>
#include <DirectXMath.h>

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

	const Vertex vertices[] =
	{
		{  0.0f,  0.5f,  255, 0, 0 },
		{  0.5f, -0.5f,  0, 255, 0 },
		{ -0.5f, -0.5f,  0, 0, 255 },
	};

	//Create VB
	wrl::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);

	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	hr = pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);
	if (FAILED(hr))
	{
		std::cerr << "CreateBuffer failed : " << hr << std::endl;
	}

	//Bind VB
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);

	//Create IB
	const unsigned short indices[] =
	{
		0, 1, 2
	};

	wrl::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned short);

	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	hr = pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer);
	if (FAILED(hr))
	{
		std::cerr << "CreateBuffer failed : " << hr << std::endl;
	}

	//Bind IB
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);

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

	//Bind IB
	pContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	
	//Create PS
	wrl::ComPtr<ID3DBlob> pBlob;
	wrl::ComPtr<ID3D11PixelShader> pPixelShader;
	D3DReadFileToBlob(L"PixelShader.cso", &pBlob);
	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

	//Bind PS
	pContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);

	//Create VS
	wrl::ComPtr<ID3D11VertexShader> pVertexShader;
	D3DReadFileToBlob(L"VertexShader.cso", &pBlob);
	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

	//Bind VS
	pContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	//Input Layout
	wrl::ComPtr<ID3D11InputLayout> layout;
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	pDevice->CreateInputLayout(
		ied,
		(UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&layout
	);

	//Bind InputLayout
	pContext->IASetInputLayout(layout.Get());

	//Bind RenderTarget
	pContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);

	//Primiive topology
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Draw
	pContext->DrawIndexed((UINT)std::size(indices), 0u, 0u);
	//pContext->Draw((UINT)std::size(vertices), 0u);
}
