#include "AppWindow.h"

#include <iostream>

#include "GraphicsEngine.h"

struct vec3
{
	float x, y, z;
};

struct vertex
{
	vec3 position;
};

AppWindow::AppWindow()
{

}

AppWindow::~AppWindow()
{
	
}


void AppWindow::onCreate()
{
	GraphicsEngine::get()->init();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = getClientWindowRect();

	std::cout << "hwnd: " << this->m_hwnd;
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);


	// compile & create vertex shader
	void* vs_byte_code = nullptr;
	size_t vs_size = 0;
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &vs_byte_code, &vs_size);
	m_vertex_shader = GraphicsEngine::get()->createVertexShader(vs_byte_code, vs_size);

	vertex list[] = {
		{-0.5f,-0.5f,0.0f},
		{-0.5f,0.5f,0.0f},
		{0.5f,-0.5f,0.0f},
		{0.5f,0.5f,0.0f},
	};
	UINT const size_list = ARRAYSIZE(list);
	m_vertex_buffer = GraphicsEngine::get()->createVertexBuffer();
	//void* shader_byte_code = nullptr;
	//UINT size_shader = 0;
	//GraphicsEngine::get()->getShaderBufferAndSize(&shader_byte_code, &size_shader);
	m_vertex_buffer->load(list, sizeof(vertex), size_list, vs_byte_code, vs_size);


	GraphicsEngine::get()->createShaders();

	GraphicsEngine::get()->releaseCompiledShader();
}

void AppWindow::onUpdate()
{
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(m_swap_chain, 0, 0, 1, 1);

	RECT rc = getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);
	GraphicsEngine::get()->setShaders();

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vertex_shader);
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vertex_buffer);
	
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_vertex_buffer->getSizeVertexList(), 0);


	m_swap_chain->present(true);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->release();
	m_vertex_buffer->release();
	m_vertex_shader->release();
	GraphicsEngine::get()->release();
}
