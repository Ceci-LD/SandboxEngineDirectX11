#pragma once
#include <d3d11.h>

//Linked -> additional dependencies -> d3d11.lib
//#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "D3DCompiler.lib")

class SwapChain;

//Singleton Class
class Graphics
{
	friend class SwapChain;
public:
	Graphics();
	~Graphics();
	static Graphics* Get();

	bool Init();
	bool Release();

	SwapChain* CreateSwapChain();

private:
	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_Context;

private:
	IDXGIDevice* m_DXGIDevice;
	IDXGIAdapter* m_DXGIAdapter;
	IDXGIFactory* m_DXGIFactory;
	D3D_FEATURE_LEVEL m_FeatureLevel;
};