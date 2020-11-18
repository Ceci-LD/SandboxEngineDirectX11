#pragma once

#include "Graphics.h"
#include <vector>

class InputLayout
{
public:
	InputLayout(Graphics& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& desc, ID3DBlob* blob)
	{
		gfx.GetDevice()->CreateInputLayout(
			desc.data(),
			(UINT)desc.size(),
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			&m_Layout
		);
	}

	~InputLayout()
	{

	}

	void Bind(Graphics& gfx)
	{
		gfx.GetContext()->IASetInputLayout(m_Layout.Get());
	}

private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_Layout;
};