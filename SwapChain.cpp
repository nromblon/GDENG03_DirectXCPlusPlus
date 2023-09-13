#include "SwapChain.h"

#include <iostream>

#include "GraphicsEngine.h"

SwapChain::SwapChain()
{
}

SwapChain::~SwapChain()
{
}

bool SwapChain::init(HWND hwnd, UINT w, UINT h)
{
	ID3D11Device* device = GraphicsEngine::get()->m_d3d_device;

	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferCount = 1;
	desc.BufferDesc.Width = w;
	desc.BufferDesc.Height = h;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BufferDesc.RefreshRate.Numerator = 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;	// Make sure to set this to 1. 
	desc.SampleDesc.Quality = 0;
	desc.OutputWindow = hwnd;
	desc.Windowed = TRUE;


	HRESULT hr = GraphicsEngine::get()->m_dxgi_factory->CreateSwapChain(device, &desc, &m_swap_chain);

	if (FAILED(hr)) {
		return false;
	}

	ID3D11Texture2D* buffer = nullptr;
	hr = m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

	if (FAILED(hr)) {
		return false;
	}

	m_render_target_view = nullptr;
	hr = device->CreateRenderTargetView(buffer, NULL, &m_render_target_view);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}

bool SwapChain::release()
{
	m_swap_chain->Release();
	delete this;
	return true;
}

bool SwapChain::present(bool vsync)
{
	HRESULT res = m_swap_chain->Present(vsync, NULL);
	if (FAILED(res))
		return false;

	return true;
}

ID3D11RenderTargetView* SwapChain::getRenderTargetView()
{
	return m_render_target_view;
}
