#include "GraphicsEngine.h"

#include <iostream>

GraphicsEngine::GraphicsEngine(): m_selected_feature_level()
{
	m_d3d_device = nullptr;
	m_imm_context = nullptr;

	m_dxgi_device = nullptr;
	m_dxgi_adapter = nullptr;
	m_dxgi_factory = nullptr;
}

GraphicsEngine::~GraphicsEngine()
{
}

bool GraphicsEngine::init()
{
	D3D_DRIVER_TYPE driver_types[] = {
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE
	};
	constexpr UINT num_driver_types = ARRAYSIZE(driver_types);

	D3D_FEATURE_LEVEL feature_levels[] = {
		D3D_FEATURE_LEVEL_11_0
	};
	constexpr UINT num_feature_levels = ARRAYSIZE(feature_levels);

	HRESULT res = 0;
	for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
	{
		res = D3D11CreateDevice(
			NULL,
			driver_types[driver_type_index],
			NULL, D3D11_CREATE_DEVICE_DEBUG,
			feature_levels, num_feature_levels,
			D3D11_SDK_VERSION,
			&m_d3d_device,
			&m_selected_feature_level,
			&m_imm_context
		);
		if (SUCCEEDED(res))
			break;

		++driver_type_index;
	}

	if (FAILED(res))
		return false;

	// Without using CreateDeviceAndSwapChain Swap chain can only be created through idxgifactory
	m_d3d_device->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgi_device);
	m_dxgi_device->GetAdapter(&m_dxgi_adapter);
	m_dxgi_adapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgi_factory);


	return true;
}

bool GraphicsEngine::release()
{
	m_imm_context->Release();
	m_d3d_device->Release();

	//// release dxgi resources
	m_dxgi_device->Release();
	m_dxgi_adapter->Release();
	m_dxgi_factory->Release();

	return true;
}

GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine instance;

	return &instance;

}

SwapChain* GraphicsEngine::createSwapChain()
{
	return new SwapChain;
}
