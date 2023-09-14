#pragma once
#include <d3d11.h>
#include "DeviceContext.h"
#include "SwapChain.h"
#include "VertexBuffer.h"

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();

	bool init();
	bool release();

	static GraphicsEngine* get();

	SwapChain* createSwapChain();
	DeviceContext* getImmediateDeviceContext();
	VertexBuffer* createVertexBuffer();
	bool createShaders();
	void setShaders();
	void getShaderBufferAndSize(void** bytecode, UINT* size);

private:
	ID3D11Device* m_d3d_device;
	D3D_FEATURE_LEVEL m_selected_feature_level;
	DeviceContext* m_imm_context;

	// for swap chain creation
	IDXGIDevice* m_dxgi_device;
	IDXGIAdapter* m_dxgi_adapter;
	IDXGIFactory* m_dxgi_factory;

	ID3DBlob* m_vs_blob;
	ID3DBlob* m_ps_blob;

	ID3D11VertexShader* m_vs;
	ID3D11PixelShader* m_ps;


	friend class SwapChain;
	friend class VertexBuffer;
};

