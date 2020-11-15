#include "SwapChain.h"
#include "Engine/Graphics/Graphics.h"

SwapChain::SwapChain()
{
}

SwapChain::~SwapChain()
{
}

bool SwapChain::Init(HWND hwnd, UINT width, UINT height)
{
	ID3D11Device* device = Graphics::Get()->m_Device;

	DXGI_SWAP_CHAIN_DESC scd = { 0 };
	scd.BufferDesc.Width = width;
	scd.BufferDesc.Height = height;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.RefreshRate.Numerator = 0;
	scd.BufferDesc.RefreshRate.Denominator = 0;
	scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scd.BufferCount = 1;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hwnd;
	scd.Windowed = TRUE;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scd.Flags = 0;

	HRESULT result = Graphics::Get()->m_DXGIFactory->CreateSwapChain(device, &scd, &m_SwapChain);

	if (FAILED(result))
	{
		return false;
	}

	return true;
}

bool SwapChain::Release()
{
	m_SwapChain->Release();
	delete this;
	return true;
}
