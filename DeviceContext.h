#pragma once
#include <d3d11.h>

class SwapChain; // Forward declaration of a class. Acts similar to #include, but can only use pointers / references to that class.

class DeviceContext
{
public:
	DeviceContext(ID3D11DeviceContext* device_context);
	~DeviceContext();

	bool clearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha);
	bool release();

private:
	ID3D11DeviceContext* m_device_context;
};

