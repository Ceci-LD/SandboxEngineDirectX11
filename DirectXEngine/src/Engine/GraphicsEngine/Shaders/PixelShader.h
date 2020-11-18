#pragma once

#include "Engine/GraphicsEngine/Graphics.h"
#include <d3dcompiler.h>
#include <string>

class PixelShader
{
public:
	PixelShader(Graphics& gfx, const std::wstring& path)
	{
		D3DReadFileToBlob(L"PixelShader.cso", &m_Blob);
		gfx.GetDevice()->CreatePixelShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_PixelShader);
	}

	~PixelShader()
	{

	}

	void Bind(Graphics& gfx)
	{
		gfx.GetContext()->PSSetShader(m_PixelShader.Get(), nullptr, 0u);
	}

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob> m_Blob;
};