#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "GraphicsEngine.h"
#include <d3dcompiler.h>
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

	ID3D11DeviceContext* imm_context;
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
			&imm_context
		);
		if (SUCCEEDED(res)) {
			m_imm_context = new DeviceContext(imm_context);
			std::cout << "D3D11Device Created: " << driver_types[driver_type_index] << std::endl;
			break;
		}

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
	m_imm_context->release();
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

DeviceContext* GraphicsEngine::getImmediateDeviceContext()
{
	return m_imm_context;
}

VertexBuffer* GraphicsEngine::createVertexBuffer()
{
	return new VertexBuffer();
}

bool GraphicsEngine::createShaders()
{
	ID3DBlob* errblob = nullptr;
	m_vs_blob = nullptr;
	m_ps_blob = nullptr;
	HRESULT hres = D3DCompileFromFile(L"shader.fx", nullptr, nullptr, "vsmain", "vs_5_0", NULL, NULL, &m_vs_blob, &errblob);
	if (FAILED(hres))
		return false;
	hres = D3DCompileFromFile(L"shader.fx", nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &m_ps_blob, &errblob);
	if (FAILED(hres))
		return false;

	m_vs = nullptr;
	hres = m_d3d_device->CreateVertexShader(m_vs_blob->GetBufferPointer(), m_vs_blob->GetBufferSize(), nullptr, &m_vs);
	if (FAILED(hres))
		return false;
	m_ps = nullptr;
	hres = m_d3d_device->CreatePixelShader(m_ps_blob->GetBufferPointer(), m_ps_blob->GetBufferSize(), nullptr, &m_ps);
	if (FAILED(hres))
		return false;

	return true;
}

void GraphicsEngine::setShaders()
{
	ID3D11DeviceContext* imm_context = m_imm_context->getDeviceContext();
	imm_context->VSSetShader(m_vs, nullptr, 0);
	imm_context->PSSetShader(m_ps, nullptr, 0);
}

void GraphicsEngine::getShaderBufferAndSize(void** bytecode, UINT* size)
{
	*bytecode = this->m_vs_blob->GetBufferPointer();
	*size = (UINT)this->m_vs_blob->GetBufferSize();
}
