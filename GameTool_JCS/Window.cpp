/*******************************************************************
*                   JCSCC_Framework Version 1.0
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*   See LICENSE.txt for modification and distribution information
*		        Copyright (c) 2016 by Shen, Jen-Chieh
******************************************************************/

#include "Window.h"


#include "GameTool_StdAfx.h"

// Input Devices
#include <GameInput_JCS\DirectInput\Keyboard.h>
#include <GameInput_JCS\DirectInput\Mouse.h>
#include "GameTimer.h"


#define WM_SOCKET 104

// Using algorithm's version, because is "inline template function"
#undef min
#undef max

namespace JCS_GameTool
{
	//WindowProc - Handles input sent to the window.
	LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_CREATE:		// 創建時...
		{
			//if (!client->initialize(hWnd)) {		// 如果創建連線失敗...
			//	SendMessage(hWnd, WM_DESTROY, NULL, NULL);
			//}
		} break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		} break;

		case WM_SOCKET:	// 連結後...
		{
			if (WSAGETSELECTERROR(lParam))
			{
				MessageBox(hWnd, L"Connection to server failed", L"Error", MB_OK | MB_ICONERROR);
				SendMessage(hWnd, WM_DESTROY, NULL, NULL);
				break;
			}
		} break;

		// ************ KEYBOARD MESSAGES ************ //
		case  WM_KEYDOWN:
		{
            JCS_GameTool::Application::GetKeyBoardServer()->OnKeyPressed(wParam);
		} break;

		case  WM_KEYUP:
		{
			JCS_GameTool::Application::GetKeyBoardServer()->OnKeyReleased(wParam);
		} break;

		case WM_CHAR:
		{
            JCS_GameTool::Application::GetKeyBoardServer()->OnChar(wParam);
		} break;

		// ************ END KEYBOARD MESSAGES ************ //

		// ************ MOUSE MESSAGES ************ //
		case WM_MOUSEMOVE:
		{
			int x = static_cast<short>(LOWORD(lParam));
			int y = static_cast<short>(HIWORD(lParam));
			int screenWidth = Window::GetMainWindow()->GetScreenWidth();
			int screenHeight = Window::GetMainWindow()->GetScreenHeight();
			if (x > 0 && x < screenWidth && y > 0 &&
				y < screenHeight)
			{
                JCS_GameTool::Application::GetMouseServer()->OnMouseMove(x, y);
                if (!JCS_GameTool::Application::GetMouseServer()->IsInWindow())
				{
					SetCapture(hWnd);
                    JCS_GameTool::Application::GetMouseServer()->OnMouseEnter();
				}
			}
			else
			{
				if (wParam & (MK_LBUTTON | MK_RBUTTON))
				{
					x = std::max(0, x);
					x = std::min(screenWidth - 1, x);
					y = std::max(0, y);
					y = std::min(screenHeight - 1, y);
                    JCS_GameTool::Application::GetMouseServer()->OnMouseMove(x, y);
				}
				else
				{
					ReleaseCapture();
                    JCS_GameTool::Application::GetMouseServer()->OnMouseLeave();
                    JCS_GameTool::Application::GetMouseServer()->OnLeftReleased();
                    JCS_GameTool::Application::GetMouseServer()->OnRightReleased();
				}
			}
		} break;

		case WM_LBUTTONDOWN:
		{
            JCS_GameTool::Application::GetMouseServer()->OnLeftPressed();
		} break;

		case WM_RBUTTONDOWN:
		{
            JCS_GameTool::Application::GetMouseServer()->OnRightPressed();
		} break;

		case WM_LBUTTONUP:
		{
            JCS_GameTool::Application::GetMouseServer()->OnLeftReleased();
		} break;

		case WM_RBUTTONUP:
		{
            JCS_GameTool::Application::GetMouseServer()->OnRightReleased();
		} break;

		// ************ END MOUSE MESSAGES ************ //
		}// end switch(message) {  }

		return DefWindowProc(hWnd, message, wParam, lParam);
	}

    // ********************* Global Variables ************************************************
	GameError* g_pThrownError = nullptr;
    Window* Window::s_pGlobalWindow = nullptr;
    Application* Window::s_pApplication = nullptr;

    Window::Window(HWND hWnd, WNDPROC wWinProc /*= WinProc*/, Application* app /*= nullptr*/)
		: mhMainWndow(hWnd)
		, mhWinProc(wWinProc)
		, m_screenWidth(0)
		, m_screenHeight(0)
		, m_windowTitle()
	{
        s_pGlobalWindow = this;
        s_pApplication = app;
	}


	Window::~Window()
	{
		SafeDeleteObject(g_pThrownError);
        s_pGlobalWindow = nullptr;
        s_pApplication = nullptr;
	}

	bool Window::GenerateWindow(HINSTANCE hInstance,
		int32 nCmdShow,
		std::wstring className,
		std::wstring windowTitle,
		int32 width, int32 height)
	{
		this->m_screenWidth = width;
		this->m_screenHeight = height;
		m_windowTitle = windowTitle;

		return GenerateWindow(hInstance,
			nCmdShow, className, windowTitle,
			(GetSystemMetrics(SM_CXSCREEN) - width) / 2,		// set to the middle of the screen
			(GetSystemMetrics(SM_CYSCREEN) - height) / 2,		// set to the middle of the screen
			width, height);
	}

	//Generic GenerateWindow function.
	bool Window::GenerateWindow(HINSTANCE hInstance,
		int32 nCmdShow,
		std::wstring className, std::wstring windowTitle,
		int32 x, int32 y, int32 width, int32 height)
	{
		WNDCLASSEX wcex;

		ZeroMemory(&wcex, sizeof(WNDCLASSEX));
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
		wcex.cbSize = sizeof(wcex);
		wcex.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = mhWinProc;
		wcex.hInstance = hInstance;
        wcex.hbrBackground = CreateSolidBrush(GetSysColor(COLOR_BACKGROUND));
		wcex.lpszClassName = className.c_str();
		wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
		wcex.hIconSm = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(s_pApplication->GetIcon()), IMAGE_ICON, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CXICON), 0);
        wcex.hIcon = (HICON)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(s_pApplication->GetIcon()), IMAGE_ICON, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CXICON), 0);

		if (!RegisterClassEx(&wcex)) 
        {
			OutputDebugString(L"\nFAILED TO CREATE WINDOW CLASS\n");
			return false;
		}

		RECT wr = { 0, 0, width, height };
		AdjustWindowRect(&wr, WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX, FALSE);
		UINT widthX = wr.right - wr.left;			// use to measure and adjust window display.. (width)
		UINT heightX = wr.bottom - wr.top;			// use to mesure and adjust window display.. (height)

		mhMainWndow = CreateWindowEx(NULL, className.c_str(), windowTitle.c_str(),
			WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX,
			x, y,
			widthX,
			heightX,
			NULL,
			NULL,
			hInstance,
			NULL);

		if (!mhMainWndow)
		{
			MessageBox(NULL, L"CreateWindow Failed.", NULL, NULL);
			return false;
		}

		ShowWindow(mhMainWndow, nCmdShow);
		UpdateWindow(mhMainWndow);

		return true;
	}


	void Window::CalculateFrameStats(GameTimer* pTimer)
	{
		// Code computes the average frames per second, and also the 
		// average time it takes to render one frame.  These stats 
		// are appended to the window caption bar.

		static int32 frameCnt = 0;
		static float32 timeElapsed = 0.0f;

		++frameCnt;

		// Compute averages over one second period.
		if ((pTimer->TotalTime() - timeElapsed) >= 1.0f)
		{
			float32 fps = static_cast<float32>(frameCnt); // fps = frameCnt / 1
			float32 mspf = 1000.0f / fps;

			std::wostringstream outs;
			outs.precision(6);
			outs << m_windowTitle << L"    "
				<< L"FPS: " << fps << L"    "
				<< L"Frame Time: " << mspf << L" (ms)";
			SetWindowText(mhMainWndow, outs.str().c_str());

			// Reset for next average.
			frameCnt = 0;
			timeElapsed += 1.0f;
		}
	}

}

