#pragma once

#include "Graphics.h"

class Topology
{
public:
	Topology(Graphics& gfx, D3D11_PRIMITIVE_TOPOLOGY type)
		: type(type)
	{
	}

	void Bind(Graphics& gfx)
	{
		gfx.GetContext()->IASetPrimitiveTopology(type);
	}

protected:
	D3D11_PRIMITIVE_TOPOLOGY type;
};