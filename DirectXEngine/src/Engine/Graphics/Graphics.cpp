#include "Graphics.h"
#include "SwapChain/SwapChain.h"

Graphics::Graphics()
{
}

Graphics::~Graphics()
{
}

bool Graphics::Init()
{
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	UINT driverTypesCount = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	UINT featureLevelsCount = ARRAYSIZE(featureLevels);

	HRESULT result;
	for (UINT i = 0; i < driverTypesCount;)
	{
		result = D3D11CreateDevice(NULL, driverTypes[i],
			NULL, 
			D3D11_CREATE_DEVICE_DEBUG,
			featureLevels,
			featureLevelsCount,
			D3D11_SDK_VERSION,
			&m_Device,
			&m_FeatureLevel,
			&m_Context);

		if (SUCCEEDED(result))
			break;

		++i;
	}

	if (FAILED(result))
	{
		return false;
	}

	//Create Swap Chain
	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_DXGIDevice);
	m_DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_DXGIAdapter);
	m_DXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_DXGIFactory);

	return true;
}

bool Graphics::Release()
{
	m_DXGIDevice->Release();
	m_DXGIAdapter->Release();
	m_DXGIFactory->Release();

	m_Context->Release();
	m_Device->Release();
	return true;
}

SwapChain* Graphics::CreateSwapChain()
{
	//Deleted in SwapChain::Release()
	return new SwapChain();
}

Graphics* Graphics::Get()
{
	static Graphics graphics;
	return &graphics;
}
