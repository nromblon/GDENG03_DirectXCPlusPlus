#include "AppWindow.h"
#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>
#include "Vector3D.h"
#include "EngineTime.h"
#include "GraphicsEngine.h"
#include "Matrix4x4.h"

struct Vertex
{
	Vector3D position;
	Vector3D color0;
	Vector3D color1;
};

__declspec(align(16))
struct constant
{
	Matrix4x4 worldMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 projMatrix;
	float m_time;
};

void AppWindow::onCreate()
{
	GraphicsEngine::get()->init();
	EngineTime::initialize();
	m_swap_chain = GraphicsEngine::get()->createSwapChain();

	RECT rc = getClientWindowRect();

	std::cout << "hwnd: " << this->m_hwnd;
	m_swap_chain->init(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);


	// compile & create Vertex shader
	void* vs_byte_code = nullptr;
	size_t vs_size = 0;
	GraphicsEngine::get()->compileVertexShader(L"VertexShader.hlsl", "vsmain", &vs_byte_code, &vs_size);
	m_vertex_shader = GraphicsEngine::get()->createVertexShader(vs_byte_code, vs_size);

	Vertex vertex_list[] = {
		// FRONT FACE
		{Vector3D(-0.5f,-0.5f,-0.5f),    Vector3D(1,0,0),  Vector3D(0.2f,0,0) },
		{Vector3D(-0.5f,0.5f,-0.5f),    Vector3D(1,1,0), Vector3D(0.2f,0.2f,0) },
		{ Vector3D(0.5f,0.5f,-0.5f),   Vector3D(1,1,0),  Vector3D(0.2f,0.2f,0) },
		{ Vector3D(0.5f,-0.5f,-0.5f),     Vector3D(1,0,0), Vector3D(0.2f,0,0) },

		//BACK FACE
		{ Vector3D(0.5f,-0.5f,0.5f),    Vector3D(0,1,0), Vector3D(0,0.2f,0) },
		{ Vector3D(0.5f,0.5f,0.5f),    Vector3D(0,1,1), Vector3D(0,0.2f,0.2f) },
		{ Vector3D(-0.5f,0.5f,0.5f),   Vector3D(0,1,1),  Vector3D(0,0.2f,0.2f) },
		{ Vector3D(-0.5f,-0.5f,0.5f),     Vector3D(0,1,0), Vector3D(0,0.2f,0) },
	};

	UINT const size_list = ARRAYSIZE(vertex_list);
	m_vertex_buffer = GraphicsEngine::get()->createVertexBuffer();
	std::cout << "size_list: " << size_list << std::endl;
	m_vertex_buffer->load(vertex_list, sizeof(Vertex), size_list, vs_byte_code, vs_size);

	unsigned int index_list[] = {
		// FRONT FACE
		0, 1, 2,
		2, 3, 0,
		// BACK FACE
		4, 5, 6,
		6, 7, 4,
		// TOP FACE
		1, 6, 5,
		5, 2, 1,
		// BOTTOM FACE
		7, 0, 3,
		3, 4, 7,
		// RIGHT FACE
		3, 2, 5,
		5, 4, 3,
		// LEFT FACE
		7, 6, 1,
		1, 0, 7
	};
	// create Index Buffer
	m_index_buffer = GraphicsEngine::get()->createIndexBuffer();
	UINT size_index_list = ARRAYSIZE(index_list);

	m_index_buffer->load(index_list, size_index_list);


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
	//ticks += EngineTime::getDeltaTime() * 2 * M_PI * 3.0f;
	ticks += EngineTime::getDeltaTime() * 1.0f;

	//std::cout << "speedFactor: " << speedFactor << "\n";
	if (speedFactor > 10.0f) // up to 3 oscillations per second
	{
		this->isIncreasing = false;
	}
	else if (speedFactor < 0.1f)
	{
		this->isIncreasing = true;
	}

	if (isIncreasing)
		this->speedFactor += 0.05f;
	else
		this->speedFactor -= 0.05f;

	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(m_swap_chain, 0.2f, 0.2f, 0.2f, 1);

	RECT rc = getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);


	constant cc;
	Matrix4x4 temp;

	cc.worldMatrix.setScale(Vector3D(1.0f, 1.0f, 1.0f));

	temp.setIdentity();
	temp.setRotationZ(ticks);
	cc.worldMatrix = cc.worldMatrix.multiplyTo(temp);

	temp.setIdentity();
	temp.setRotationY(ticks);
	cc.worldMatrix = cc.worldMatrix.multiplyTo(temp);

	temp.setIdentity();
	temp.setRotationX(ticks);
	cc.worldMatrix = cc.worldMatrix.multiplyTo(temp);


	cc.viewMatrix.setIdentity();
	cc.projMatrix.setOrthoLH
	(
		(rc.right - rc.left) / 300.0f,
		(rc.bottom - rc.top) / 300.0f,
		-4.0f,
		4.0f
	);


	cc.m_time = this->ticks;	

	//std::cout << "m_time: " << cc.m_time << "\n";
	//std::cout << "sin(m_time): " << (std::sin(cc.m_time) + 1) / 2 << "\n";
	m_constant_buffer->update(GraphicsEngine::get()->getImmediateDeviceContext(), &cc);

	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_vertex_buffer);
	GraphicsEngine::get()->getImmediateDeviceContext()->setIndexBuffer(m_index_buffer);

	// Set Default Shaders
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_vertex_shader);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_pixel_shader);

	// Bind constant buffers to the shaders
	GraphicsEngine::get()->getImmediateDeviceContext()->SetConstantBuffer(m_vertex_shader, m_constant_buffer);
	GraphicsEngine::get()->getImmediateDeviceContext()->SetConstantBuffer(m_pixel_shader, m_constant_buffer);

	std::cout << "index buffer size: " << m_index_buffer->getIndexSize() << std::endl;
	GraphicsEngine::get()->getImmediateDeviceContext()->drawIndexedTriangleList(m_index_buffer->getIndexSize(), 0, 0);


	m_swap_chain->present(true);
	//std::cout << EngineTime::getDeltaTime() << std::endl;
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_swap_chain->release();
	m_vertex_buffer->release();
	m_index_buffer->release();
	m_constant_buffer->release();

	m_vertex_shader->release();
	m_pixel_shader->release();
	GraphicsEngine::get()->release();
}

AppWindow::AppWindow()
{

}

AppWindow::~AppWindow()
{

}
