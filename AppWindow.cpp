#include "AppWindow.h"
#include "GraphicsEngine.h"

AppWindow::AppWindow() = default;

AppWindow::~AppWindow() = default;


void AppWindow::onCreate()
{
	GraphicsEngine::get()->init();
}

void AppWindow::onUpdate()
{
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	GraphicsEngine::get()->release();
}
