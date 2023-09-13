#pragma once
#include "d3d11.h"

class SwapChain
{
public:
	SwapChain();
	~SwapChain();

	bool init(HWND hwnd, UINT w, UINT h);
	bool release();

private:
	IDXGISwapChain* m_swap_chain;
};

