#pragma once
#include <vector>

#include "Cube.h"
#include "InputListener.h"
#include "Window.h"
#include "SwapChain.h"
#include "VertexBuffer.h"
#include "VertexShader.h"

class AppWindow: public Window, public InputListener
{
public:
	AppWindow();
	~AppWindow();

	virtual void onCreate() override;
	virtual void onUpdate() override;
	virtual void onDestroy() override;


	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
private:
	std::vector<Cube*> cubeList;

	float ticks = 0.0f;

	SwapChain* m_swap_chain;
	VertexBuffer* m_vertex_buffer;
	ConstantBuffer* m_constant_buffer;
	IndexBuffer* m_index_buffer;

	VertexShader* m_vertex_shader;
	PixelShader* m_pixel_shader;
};

