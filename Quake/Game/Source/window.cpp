#include "../Include/window.h"

/*
==================================

WINDOW

==================================
*/

namespace sys
{
	Window::Window(void)
		: _glfwWindow(nullptr), _title(), _size(), _isOpen(false)
	{
	}
	Window::~Window(void)
	{
		close();
	}
	bool Window::create(glm::ivec2 size, const std::string& title)
	{
		_glfwWindow = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
		if (_glfwWindow)
		{
			_isOpen = true;
			_size = size;
			_title = title;
			glfwMakeContextCurrent(_glfwWindow);
			return true;
		}
		else
		{
			return false;
		}
	}
	bool Window::create(int width, int height, const std::string& title)
	{
		return create(glm::ivec2(width, height), title);
	}
	void Window::close(void)
	{
		if (_glfwWindow)
		{
			_isOpen = false;
			glfwDestroyWindow(_glfwWindow);
			_glfwWindow = nullptr;
		}
	}
	void Window::pollEvents(void)
	{
		glfwPollEvents();
	}
	void Window::swapBuffers(void)
	{
		if (_glfwWindow)
		{
			glfwSwapBuffers(_glfwWindow);
		}
	}
	void Window::setTitle(const std::string& title)
	{
		_title = title;
		if (_glfwWindow)
		{
			glfwSetWindowTitle(_glfwWindow, title.c_str());
		}
	}
	void Window::setSize(glm::ivec2 size)
	{
		_size = size;
		if (_glfwWindow)
		{
			glfwSetWindowSize(_glfwWindow, size.x, size.y);
		}
	}
	void Window::setWidth(int width)
	{
		setSize(glm::ivec2(width, _size.y));
	}
	void Window::setHeight(int height)
	{
		setSize(glm::ivec2(_size.x, height));
	}
	GLFWwindow* Window::getWindow(void) const
	{
		return _glfwWindow;
	}
	const std::string& Window::getTitle(void) const
	{
		return _title;
	}
	glm::vec2 Window::getSize(void) const
	{
		return _size;
	}
	int Window::getWidth(void) const
	{
		return _size.x;
	}
	int Window::getHeight(void) const
	{
		return _size.y;
	}
	bool Window::isOpen(void) const
	{
		return _isOpen;
	}

	bool Window::shouldClose(void) const
	{
		if (_glfwWindow) {
			return glfwWindowShouldClose(_glfwWindow);
		}
		return true; // Close if window not initialized
	}
}