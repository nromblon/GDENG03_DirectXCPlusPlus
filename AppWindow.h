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
	// Temp properties for increasing / decreasing animation
	float ticks = 0.0f;
	float speedFactor = 0.1f;
	bool isIncreasing = true;

	SwapChain* m_swap_chain;
	VertexBuffer* m_vertex_buffer;
	ConstantBuffer* m_constant_buffer;

	VertexShader* m_vertex_shader;
	PixelShader* m_pixel_shader;
};

