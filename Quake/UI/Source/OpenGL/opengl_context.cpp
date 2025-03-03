#include "..\..\Include\OpenGL\opengl_context.h"

QBool OpenGLContext::_isRegistered = false;
QBool OpenGLContext::_isInitialized = false;

void OpenGLContext::registerClass(HINSTANCE hInstance)
{
	if (!_isRegistered)
	{
		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
		wc.lpfnWndProc = (WNDPROC)classHandler;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
		wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_MENUBAR + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = TEXT(CLASS_NAME);

		if (!RegisterClassEx(&wc))
		{
			MessageBox(NULL, TEXT("Failed to register window class."), TEXT("Error"), MB_ICONERROR);
			return;
		}
		_isRegistered = true;
	}
}

QBool OpenGLContext::_initGLEW(HINSTANCE hInstance)
{
	if (!_isInitialized)
	{
		registerClass(hInstance);
		HWND wndFake = CreateWindow(TEXT(CLASS_NAME), TEXT("FAKE"), WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN,
			0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
		if (!wndFake)
		{
			MessageBox(NULL, TEXT("Failed to create fake window."), TEXT("Error"), MB_ICONERROR);
			return false;
		}

		_hdc = GetDC(wndFake);

		PixelFormatDescriptor pfd;
		std::memset(&pfd, 0, sizeof(PixelFormatDescriptor));
		pfd.nSize = sizeof(PixelFormatDescriptor);
		pfd.nVersion = 3;
		pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 32;
		pfd.cDepthBits = 32;
		pfd.iLayerType = PFD_MAIN_PLANE;

		int pixelFormat = ChoosePixelFormat(_hdc, &pfd);
		if (!pixelFormat || !SetPixelFormat(_hdc, pixelFormat, &pfd))
		{
			MessageBox(NULL, TEXT("Failed to set pixel format."), TEXT("Error"), MB_ICONERROR);
			ReleaseDC(wndFake, _hdc);
			DestroyWindow(wndFake);
			return false;
		}

		HGLRC hrcFake = wglCreateContext(_hdc);
		if (!hrcFake)
		{
			MessageBox(NULL, TEXT("Failed to create fake OpenGL context."), TEXT("Error"), MB_ICONERROR);
			ReleaseDC(wndFake, _hdc);
			DestroyWindow(wndFake);
			return false;
		}

		wglMakeCurrent(_hdc, hrcFake);
		bool result = true;
		if (glewInit() != GLEW_OK)
		{
			MessageBox(NULL, TEXT("Could not initialize GLEW."), TEXT("Fatal Error"), MB_ICONERROR);
			result = false;
		}
		else
		{
			_isInitialized = true;
		}

		wglMakeCurrent(NULL, NULL);
		wglDeleteContext(hrcFake);
		ReleaseDC(wndFake, _hdc);
		DestroyWindow(wndFake);
		return result;
	}
	return true;
}