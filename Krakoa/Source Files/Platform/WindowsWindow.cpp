#include <Precompile.hpp>
#include <Platform/WindowsWindow.hpp>

#include <Maths/kAlgorithms.hpp>

void krakoa::WindowsWindow::OnUpdate()
{
}

kMaths::Vector2u& krakoa::WindowsWindow::GetDimensions()
{
	return data.dimensions;
}

unsigned krakoa::WindowsWindow::GetWidth() const
{
	return data.dimensions.X();
}

unsigned krakoa::WindowsWindow::GetHeight() const
{
	return data.dimensions.Y();
}

void krakoa::WindowsWindow::SetEventCallback(const EventCallbackFunc& cb)
{
}

void krakoa::WindowsWindow::SetVsync(bool isEnabled)
{
}

bool krakoa::WindowsWindow::IsVsyncActive() const
{
	return false;
}
