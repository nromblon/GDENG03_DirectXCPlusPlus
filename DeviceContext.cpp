#include "DeviceContext.h"

#include "SwapChain.h"
#include "VertexBuffer.h"
#include "VertexShader.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* device_context)
{
	m_device_context = device_context;
}

void DeviceContext::clearRenderTargetColor(SwapChain* swap_chain, float red, float green, float blue, float alpha)
{
	const FLOAT clear_color[] = { red, green, blue, alpha };

	ID3D11RenderTargetView* rtv = swap_chain->getRenderTargetView();

	m_device_context->ClearRenderTargetView(rtv, clear_color);
	m_device_context->OMSetRenderTargets(1, &rtv, NULL);

}

bool DeviceContext::release()
{
	m_device_context->Release();
	delete this;

	return true;
}

void DeviceContext::setVertexBuffer(VertexBuffer* vertex_buffer)
{
	UINT stride = vertex_buffer->m_size_vertex;
	UINT offset = 0;

	m_device_context->IASetVertexBuffers(0, 1, &vertex_buffer->m_buffer, &stride, &offset);
	m_device_context->IASetInputLayout(vertex_buffer->m_input_layout);
}

void DeviceContext::setVertexShader(VertexShader* vertex_shader)
{
	m_device_context->VSSetShader(vertex_shader->getShader(), nullptr, 0);
}

void DeviceContext::drawTriangleList(UINT vertex_count, UINT start_vertex_index)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::drawTriangleStrip(UINT vertex_count, UINT start_vertex_index)
{
	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	m_device_context->Draw(vertex_count, start_vertex_index);
}

void DeviceContext::setViewportSize(UINT width, UINT height)
{
	D3D11_VIEWPORT vp= {};
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	m_device_context->RSSetViewports(1, &vp);
}

ID3D11DeviceContext* DeviceContext::getDeviceContext()
{
	return m_device_context;
}

DeviceContext::~DeviceContext()
{

}
