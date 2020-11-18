#pragma once

#include "Engine/GraphicsEngine/Graphics.h"
#include <string>
#include <d3dcompiler.h>

class VertexShader
{
public:
	VertexShader(Graphics& gfx, const std::wstring& path)
	{
		D3DReadFileToBlob(path.c_str(), &m_Blob);
		gfx.GetDevice()->CreateVertexShader(m_Blob->GetBufferPointer(), m_Blob->GetBufferSize(), nullptr, &m_VertexShader);
	}

	~VertexShader()
	{

	}

	void Bind(Graphics& gfx)
	{
		gfx.GetContext()->VSSetShader(m_VertexShader.Get(), nullptr, 0u);
	}

	ID3DBlob* GetBlob() const { return m_Blob.Get(); }

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> m_Blob;

};