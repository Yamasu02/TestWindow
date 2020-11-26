#pragma once
#include <Windows.h>
#include <iostream>
#include "CreateResources.h"
#include "Button.h"


std::wstring str;
POINT p;

class Window
{
public:

	std::wstring ClassName = L"DefaultClassName";
	WNDCLASSEXW wc = { 0 };
	HINSTANCE hInstance;
	MSG msg;

	HWND hwnd;
	static inline KeyBoard* kb;
	static inline Mouse* mouse;
	static inline CreateResources* c;
	static inline const D3DCOLORVALUE color = { 0, 1, 0.5, 0.7 };
	static inline Button* b;


	Window(std::wstring WindowTitle, WNDPROC WinProcPtr, std::wstring ClassName = L"DefaultClassName"
		, int StartX = 0, int StartY = 0, int width = 800, int height = 600
		, HICON hIcon = nullptr, DWORD style = WS_OVERLAPPEDWINDOW)
	{
		this->ClassName = ClassName;
		wc.cbSize = sizeof(wc);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = (WNDPROC)WinProcPtr;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = hIcon;
		wc.hCursor = NULL;
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = (LPCWSTR)&ClassName;
		wc.hIconSm = hIcon;
		RegisterClassEx(&wc);
		hwnd = CreateWindowEx(0, (LPCWSTR)&ClassName, WindowTitle.data(), style, StartX, StartY, width, height, nullptr, nullptr, hInstance, nullptr);
		mouse = new Mouse(hwnd);
		kb = new KeyBoard();
		c = new CreateResources(hwnd);
		//input = new Input(hwnd);
		ShowWindow(hwnd, SW_SHOW);
		SetFocus(hwnd);
		auto of = c->pRenderTarget.Get();
		//auto xt = new Button(c->pRenderTarget.Get(),)
		b = new Button(c->pRenderTarget.Get(), c->Brushes.pBlackBrush, std::wstring(L"k"), c->WriteFormats.pTextFormatNormal,
			c->WriteFormats.pTextFormatNormal, c->Brushes.pWhiteBrush, DWRITE_TEXT_ALIGNMENT_LEADING, 0);
	}

	static void PaintWindow()
	{
		c->pRenderTarget->BeginDraw();
		c->pRenderTarget->Clear(color);
		b->DrawButton(15, 22);
		c->pRenderTarget->EndDraw();
	}

	static LRESULT WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
	{
		switch (Msg)
		{
		case  WM_LBUTTONDOWN:
			mouse->ButtonStates.find(L"LeftClick")->second = true;
			break;

		case WM_LBUTTONUP:
			mouse->ButtonStates.find(L"LeftClick")->second = false;
			break;

		case WM_RBUTTONDOWN:
			mouse->ButtonStates.find(L"RightClick")->second = true;
			break;

		case WM_RBUTTONUP:
			mouse->ButtonStates.find(L"RightClick")->second = false;
			break;

		case WM_MBUTTONDOWN:
			mouse->ButtonStates.find(L"MiddleButton")->second = true;
			break;

		case WM_MBUTTONUP:
			mouse->ButtonStates.find(L"MiddleButton")->second = false;
			break;

		case WM_MOUSEMOVE:

			mouse->WindowPos = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			break;

		case WM_CHAR:

			kb->KeyPressed.KeyCode = static_cast<std::byte>(wParam);
			kb->KeyPressed.lParam = (std::bitset<32>)lParam;
			//ooff = kb->KeyPressed.KeyCode;
			break;

		case WM_KEYDOWN:

			kb->KeyPressed.KeyCode = static_cast<std::byte>(wParam);
			kb->KeyPressed.lParam = (std::bitset<32>)lParam;
			//ooff = kb.KeyPressed.lParam;

		case WM_SYSKEYDOWN:

			kb->KeyPressed.KeyCode = static_cast<std::byte>(wParam);
			kb->KeyPressed.lParam = (std::bitset<32>)lParam;
			//ooff = kb.KeyPressed.lParam;

		case WM_PAINT:

			PaintWindow();
			//D3Dptr->DrawTriangle(hWnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, Msg, wParam, lParam);
		}
	}

	void MsgLoop()
	{
		while (1)
		{
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				mouse->Update();
				p = mouse->ScreenPos;
				//str = std::to_wstring(p.x);
				//SetWindowText(hwnd, (LPCWSTR)&str);
				if (GetAsyncKeyState(VK_TAB))
				{
					//input->kb.IntializeArtificialInputKb(0x41);
					//input->kb.KeyDown();
					//input->kb.KeyUp();
					//input->mouse->RightClick();
					mouse->MoveMouse(1600, 20);
					//input->mouse->LeftClick();
				}
				/*if (input->mouse->ButtonStates.find(L"LeftClick")->second == true)
				{
					break;
				}*/
				if (mouse->ButtonStates.find(L"RightClick")->second == true)
				{
					//break;
				}
				kb->KeyPressed.UpdateRepeatCount();
				if (kb->KeyPressed.KeyCode == (std::byte)'a' && kb->KeyPressed.RepeatCount >= 10)
				{
					auto of = kb->KeyPressed.RepeatCount;
					auto xt = 'a';
				}

				if (msg.message == WM_QUIT)
				{
					break;
				}

				TranslateMessage(&msg);
				DispatchMessage(&msg);
				UpdateWindow(hwnd);
				Sleep(2);
			}
			break;
		}
	}


	~Window()
	{
		UnregisterClass(ClassName.c_str(), hInstance);
	}
};