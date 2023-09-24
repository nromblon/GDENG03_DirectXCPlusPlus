#pragma once
#include "Window.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "VertexShader.h"

class AppWindow: public Window
{
public:
	AppWindow();
	~AppWindow();

	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;
private:
	SwapChain* m_swap_chain;
	VertexBuffer* m_vertex_buffer;
	VertexShader* m_vertex_shader;
};

