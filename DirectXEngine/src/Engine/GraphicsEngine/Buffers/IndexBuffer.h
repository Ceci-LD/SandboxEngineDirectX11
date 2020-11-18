#pragma once

#include "Engine/GraphicsEngine/Graphics.h"
#include <vector>

class IndexBuffer
{
public:
	IndexBuffer(Graphics& gfx, const std::vector<unsigned short>& indices)
		: m_Count((UINT)indices.size())
	{
		HRESULT hr;

		D3D11_BUFFER_DESC ibd = {};
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.CPUAccessFlags = 0u;
		ibd.MiscFlags = 0u;
		ibd.ByteWidth = m_Count * sizeof(unsigned short);
		ibd.StructureByteStride = sizeof(unsigned short);

		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = indices.data();
		hr = gfx.GetDevice()->CreateBuffer(&ibd, &isd, &m_IndexBuffer);
		if (FAILED(hr))
		{
			std::cerr << "FAILED to create [IndexBuffer]!\n";
		}
	}

	~IndexBuffer()
	{
	}

	void Bind(Graphics& gfx)
	{
		gfx.GetContext()->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	}

	UINT GetCount() { return m_Count; }

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;
	UINT m_Count;
};