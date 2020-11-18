#pragma once

#include "Engine/GraphicsEngine/Graphics.h"
#include <vector>

class VertexBuffer
{
public:
	template<class V>
	VertexBuffer(Graphics& gfx, const std::vector<V>& vertices)
		: m_Stride(sizeof(V))
	{
		HRESULT hr;

		D3D11_BUFFER_DESC bd = {};
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0u;
		bd.MiscFlags = 0u;
		bd.ByteWidth = static_cast<UINT>(sizeof(V) * vertices.size());
		bd.StructureByteStride = m_Stride;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();
		hr = gfx.GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
		if (FAILED(hr))
		{
			std::cerr << "FAILED to create [VertexBuffer]!\n";
		}
	}

	~VertexBuffer()
	{
	}

	void Bind(Graphics& gfx)
	{
		const UINT offset = 0u;
		gfx.GetContext()->IASetVertexBuffers(0u, 1u, m_VertexBuffer.GetAddressOf(), &m_Stride, &offset);
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	UINT m_Stride;
};