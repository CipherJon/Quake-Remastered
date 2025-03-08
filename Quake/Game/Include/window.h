#pragma once

/*OpenGL Libraries*/
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

/*Standard Libraries*/
#include <string>

/*Custom Library*/
#include "../../common.h"

/*
==================================

WINDOW

==================================
*/

namespace sys
{
#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT	600

	class Window
	{
	public:
		// Constructor
		Window(void);

		// Destructor
		~Window(void);

		// Create a window with specified size and title
		bool create(glm::ivec2 size, const std::string& title);
		bool create(int width, int height, const std::string& title);

		// Close the window
		void close(void);

		// Poll for and process events
		void pollEvents(void);

		// Swap front and back buffers
		void swapBuffers(void);

		// Set window title
		void setTitle(const std::string& title);

		// Set window size
		void setSize(glm::ivec2 size);
		void setWidth(int width);
		void setHeight(int height);

		// Getters
		GLFWwindow* getWindow(void) const;
		const std::string& getTitle(void) const;
		glm::vec2 getSize(void) const;
		int getWidth(void) const;
		int getHeight(void) const;
		bool isOpen(void) const;
		bool shouldClose(void) const;

	private:
		GLFWwindow*				_glfwWindow;
		std::string				_title;
		glm::ivec2				_size;
		bool					_isOpen;
	};
}