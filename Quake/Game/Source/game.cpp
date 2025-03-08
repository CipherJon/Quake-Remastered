#include "../Include/game.h"
#include <Windows.h>
#include <array>
#include <stdexcept>
#include <thread>
#include <vector>
#include <string>

namespace sys
{
	using StringVec = std::vector<std::string>;

	std::string workingDir(void);
	StringVec parseCmdLine(char** argv, UINT32 argc);

	// Function to get the current working directory
	std::string workingDir(void)
	{
		wchar_t dir[1024] = L"";
		GetCurrentDirectoryW(static_cast<DWORD>(std::size(dir)), dir);
		std::string narrowStr;
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, dir, -1, NULL, 0, NULL, NULL);
		narrowStr.resize(size_needed);
		WideCharToMultiByte(CP_UTF8, 0, dir, -1, &narrowStr[0], size_needed, NULL, NULL);
		return narrowStr;
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
} // namespace sys

/*
==================================

Game

==================================
*/

// Constructor for the Game class
// GLFW error callback
void glfwErrorCallback(int error, const char* description) {
	// Log error instead of throwing to prevent exception propagation through C callbacks
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

Game::Game(const sys::StringVec& argv)
	: _argv(argv), _window()
{
	if (!glfwInit()) {
		const char* description;
		glfwGetError(&description);
		throw std::runtime_error("GLFW init failed: " + std::string(description));
	}

	try {
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

		if (!_window.create(1280, 720, "Quake 1.0")) { // Default 1280x720 resolution
			throw std::runtime_error("Failed to create GLFW window");
		}

		glfwSwapInterval(1); // Enable vsync
	} catch (...) {
		glfwTerminate();
		throw;
	}
}

Game::~Game() {
	glfwTerminate();
}

void Game::run()
{
	double previousTime = glfwGetTime();
	constexpr double targetFrameTime = 1.0 / 60.0; // 60 FPS

	while (_window.isOpen()) {
		const double currentTime = glfwGetTime();
		const double deltaTime = currentTime - previousTime;
		
		// Process input
		_window.pollEvents();
		// Handle continuous input
		if (_window.shouldClose()) { // Fixed potential null window reference
			break;
		}

		// Update game state
		update(deltaTime);

		// Render frame
		render();
		_window.swapBuffers();

		// Frame pacing
		// Precise frame pacing using spinlock
		while (glfwGetTime() - currentTime < targetFrameTime) {
			// Yield to other threads to avoid burning CPU
			std::this_thread::yield();
		}
		
		previousTime = currentTime;
	}
}

void Game::update(double deltaTime) {
	// Game logic implementation
}

void Game::render() {
	// Rendering implementation
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}