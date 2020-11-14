#pragma once
#include <d3d11.h>

//Linked -> additional dependencies -> d3d11.lib
//#pragma comment(lib, "d3d11.lib")
//#pragma comment(lib, "D3DCompiler.lib")

//Singleton Class
class Graphics
{
public:
	Graphics();
	~Graphics();

	bool Init();
	bool Release();

	static Graphics* Get();

private:
	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_Context;

	D3D_FEATURE_LEVEL m_FeatureLevel;
};