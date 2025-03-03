#include "../Include/game.h"

namespace sys
{
	// Function to get the current working directory
	std::string workingDir(void)
	{
		char dir[1024] = "";
		GetCurrentDirectory(sizeof(dir), dir);
		return std::string(dir);
	}

	// Function to parse command line arguments
	StringVec parseCmdLine(char** argv, UINT32 argc)
	{
		StringVec vec;
		for (UINT32 i = 1; i < argc; i++)
		{
			vec.push_back(argv[i]);
		}
		return vec;
	}
}

/*
==================================

Game

==================================
*/

// Constructor for the Game class
Game::Game(const StringVec& argv)
	: _argv(argv), _window()
{
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	if (!_window.create(WINDOW_WIDTH, WINDOW_HEIGHT, "Quake 1.0")) {
		std::cerr << "Failed to create window" << std::endl;
		glfwTerminate();
		return;
	}
}

// Initialize the game
void Game::init(void)
{
	double newTime = 0.0;
	double oldTime = 0.0;
	double deltaTime = 0.0;

	while (_window.isOpen())
	{
		newTime = glfwGetTime();
		deltaTime = newTime - oldTime;
		oldTime = newTime;

		// Poll for and process events
		_window.pollEvents();

		// Update game logic here

		// Render the game here

		// Swap front and back buffers
		_window.swapBuffers();
	}

	glfwTerminate();
}