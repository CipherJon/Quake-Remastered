#pragma once

/*Standard Library*/
#include <Windows.h>
#include <map>
#include <iostream>

/*Custom Library*/
#include "../../common.h"
#include "window.h"

/*
==================================

Game

==================================
*/

class Game
{
public:
	// Constructor
	explicit Game(const StringVec& argv);

	// Initialize the game
	void init(void);

	// Register a variable
	void registerVariable(Cvar variable);

private:
	StringVec _argv;       // Command-line arguments
	sys::Window _window;   // Game window
};
