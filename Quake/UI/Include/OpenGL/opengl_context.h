#pragma once

#include <windows.h>

#include <GL/glew.h>
#include <GL/wglew.h>

#include "../../../Container/Include/qbool.h"
#include "../../../Math/Include/vector2.hpp"

#include "../Windows/windows_defs.h"

#define CLASS_NAME "OpenGL Class"

class OpenGLContext
{
public:
	OpenGLContext(HINSTANCE hInstance, HWND* hwnd, int major, int minor);

	static void registerClass(HINSTANCE hInstance);
	static void unregisterClass(HINSTANCE hInstance);

	void makeCurrent();
	void swapBuffers();

private:
	QBool _initGLEW(HINSTANCE hInstance);

	void (*InitScene)(LPVOID lpParam) = nullptr;
	void (*RenderScene)(LPVOID lpParam) = nullptr;
	void (*ReleaseScene)(LPVOID lpParam) = nullptr;

	HDC _hdc;
	HWND* _hwnd;
	HGLRC _hrc;
	int _majorVersion;
	int _minorVersion;
	static QBool _isRegistered;
	static QBool _isInitialized;

	GLuint _vao;
	GLuint _vbo;
};

LRESULT CALLBACK classHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);