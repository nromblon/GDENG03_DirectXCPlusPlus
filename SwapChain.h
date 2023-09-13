#pragma once
#include "d3d11.h"

class SwapChain
{
public:
	SwapChain();
	~SwapChain();

	bool init(HWND hwnd, UINT w, UINT h);
	bool release();

	bool present(bool vsync);

	ID3D11RenderTargetView* getRenderTargetView();

private:
	IDXGISwapChain* m_swap_chain;
	ID3D11RenderTargetView* m_render_target_view;
};

