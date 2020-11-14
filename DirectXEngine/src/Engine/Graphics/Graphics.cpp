#include "Graphics.h"

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
			NULL, NULL, 
			featureLevels, featureLevelsCount,
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

	return true;
}

bool Graphics::Release()
{
	m_Context->Release();
	m_Device->Release();
	return true;
}

Graphics* Graphics::Get()
{
	static Graphics graphics;
	return &graphics;
}
