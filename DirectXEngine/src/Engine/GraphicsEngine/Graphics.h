#pragma once

#include <d3d11.h>

#include <wrl.h>

class Graphics
{
public:
	Graphics(HWND hwnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics();

	void ClearBuffer(float r, float g, float b);
	void EndFrame();

	void DrawTriangle(float angle, float x, float y);

	inline ID3D11Device* GetDevice() { return pDevice.Get(); }
	inline ID3D11DeviceContext* GetContext() { return pContext.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
};
