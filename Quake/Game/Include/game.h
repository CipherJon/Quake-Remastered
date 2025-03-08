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
	explicit Game(const sys::StringVec& argv);
	~Game();

	void run();
	
private:
	void update(double deltaTime);
	void render();
	
	sys::StringVec _argv;
	sys::Window _window;
};
