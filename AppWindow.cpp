#include "AppWindow.h"

#include <iostream>

#include "GraphicsEngine.h"

struct vec3
{
	float x, y, z;
};

struct vertex
{
	vec3 position0;
	vec3 position1;
	vec3 color0;
	vec3 color1;
};

__declspec(align(16))
struct constant
{
	unsigned int m_time;
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
		{-0.5f,-0.5f,0.0f, -0.32f, -0.11f, 0.0f,   1, 0, 0,      0, 0, 1},
		{-0.5f,0.5f,0.0f, -0.11f, 0.78f, 0.0f,     0, 1, 0,      1, 0, 0},
		{0.5f,-0.5f,0.0f, 0.75f, -0.73f, 0.0f,     0, 0, 1,      0, 1, 0},
		{0.5f,0.5f,0.0f, 0.88f, 0.77f, 0.0f,       1, 1, 0,      0, 1, 1},
	};
	UINT const size_list = ARRAYSIZE(list);
	m_vertex_buffer = GraphicsEngine::get()->createVertexBuffer();
	m_vertex_buffer->load(list, sizeof(vertex), size_list, vs_byte_code, vs_size);


	// compile & create pixel shader
	void* ps_byte_code = nullptr;
	size_t ps_size = 0;
	GraphicsEngine::get()->compilePixelShader(L"PixelShader.hlsl", "psmain", &ps_byte_code, &ps_size);
	m_pixel_shader = GraphicsEngine::get()->createPixelShader(ps_byte_code, ps_size);

	GraphicsEngine::get()->releaseCompiledShader();

	// Constant Buffer setup
	constant cc;
	cc.m_time = 0;
	m_constant_buffer = GraphicsEngine::get()->createConstantBuffer();
	m_constant_buffer->load(&cc, sizeof(constant)); // load initial value and format



}

void AppWindow::onUpdate()
{
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(m_swap_chain, 0.2f, 0.2f, 0.2f, 1);

	RECT rc = getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vertex_buffer);

	// Set Default Shaders
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vertex_shader);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_pixel_shader);

	constant cc;
	cc.m_time = GetTickCount();	// elapsed time in ms since the app started
	m_constant_buffer->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);

	// Bind constant buffers to the shaders
	GraphicsEngine::get()->getImmediateDeviceContext()->SetConstantBuffer(m_vertex_shader, m_constant_buffer);
	GraphicsEngine::get()->getImmediateDeviceContext()->SetConstantBuffer(m_pixel_shader, m_constant_buffer);
	
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
