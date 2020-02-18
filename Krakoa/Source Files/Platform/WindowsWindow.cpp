#include <Precompile.hpp>
#include "WindowsWindow.hpp"

void krakoa::WindowsWindow::OnUpdate()
{
}

kMaths::Vector2u& krakoa::WindowsWindow::GetDimensions()
{
	const auto s = sizeof data.dimensions;
	auto l = kMaths::VectorNormalize<Vector2u>(data.dimensions = kMaths::Vector2u());
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
