#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <array>
#include <unordered_map>
#include <cstddef>


inline std::wstring Wstr(const wchar_t* str)
{
	return std::wstring(str);
}

class Mouse
{
public:

	static inline POINT WindowPos = { 0,0 };
	static inline POINT ScreenPos = { 0,0 };
	static inline const std::array<std::wstring, 3> Buttons = { Wstr(L"RightClick"), Wstr(L"RightClick"), Wstr(L"RightClick") };
	static inline std::unordered_map<std::wstring, bool> ButtonStates;
	static inline RECT WinRect;
	static inline HWND hwnd;

	Mouse() = delete;

	Mouse(Mouse& m) = delete;

	void operator = (Mouse& m) = delete;

	Mouse(const HWND& hwnd) noexcept
	{
		this->hwnd = hwnd;
		GetCursorPos((LPPOINT)&WindowPos);
		ScreenToClient(hwnd, &WindowPos);
		ButtonStates.insert(std::make_pair<std::wstring, bool>(L"LeftClick", GetKeyState(VK_LBUTTON)));
		ButtonStates.insert(std::make_pair<std::wstring, bool>(L"RightClick", GetKeyState(VK_RBUTTON)));
		ButtonStates.insert(std::make_pair<std::wstring, bool>(L"MiddleButton", GetKeyState(VK_MBUTTON)));
	}

	static void Update()
	{
		GetClientRect(hwnd, &WinRect);
		GetCursorPos(&ScreenPos);
	}

	static POINT GetMouseDeltaPos(const POINT& OldPos, const POINT& NewPos)
	{
		return { NewPos.x - OldPos.x,NewPos.y - OldPos.y };
	}

	static bool PointBelongsToRect(const RECT& Rect, const POINT& point)
	{
		if (point.x >= Rect.left && point.x <= Rect.right && point.y >= Rect.top && point.y <= Rect.bottom)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	static BOOL CursorBelongsToRect()
	{
		return PointBelongsToRect(WinRect, WindowPos);
	}

	void MoveMouse(int x, int y, bool realtive = false)
	{
		INPUT input;
		input.type = INPUT_MOUSE;

		double fScreenWidth = GetSystemMetrics(SM_CXSCREEN) - 1;
		double fScreenHeight = GetSystemMetrics(SM_CYSCREEN) - 1;
		double fx = x * (65535.0f / fScreenWidth);
		double fy = y * (65535.0f / fScreenHeight);

		input.mi.dx = fx;
		input.mi.dy = fy;
		input.mi.mouseData = 0;
		input.mi.dwFlags = realtive ? MOUSEEVENTF_MOVE : MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE;
		input.mi.time = 0;
		input.mi.dwExtraInfo = 0;
		SendInput(1, &input, sizeof(INPUT));
	}

	void RightClick()
	{
		INPUT input;
		input.type = INPUT_MOUSE;
		input.mi.mouseData = 0;
		input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
		input.mi.time = 0;
		input.mi.dwExtraInfo = 0;
		SendInput(1, &input, sizeof(INPUT));
		input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
		SendInput(1, &input, sizeof(INPUT));
	}

	void LeftClick()
	{
		INPUT input;
		input.type = INPUT_MOUSE;
		input.mi.mouseData = 0;
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		input.mi.time = 0;
		input.mi.dwExtraInfo = 0;
		SendInput(1, &input, sizeof(INPUT));
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
		SendInput(1, &input, sizeof(INPUT));
	}

	void MiddleClick()
	{
		INPUT input;
		input.type = INPUT_MOUSE;
		input.mi.mouseData = 0;
		input.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
		input.mi.time = 0;
		input.mi.dwExtraInfo = 0;
		SendInput(1, &input, sizeof(INPUT));
		input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
		SendInput(1, &input, sizeof(INPUT));
	}

	void MoveMouseWheel(int z)
	{
		INPUT input;
		input.mi.mouseData = 0;
		input.mi.dwFlags = MOUSEEVENTF_HWHEEL;
		input.mi.time = 0;
		input.mi.dwExtraInfo = 0;
		SendInput(1, &input, sizeof(INPUT));
		input.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
		SendInput(1, &input, sizeof(INPUT));
	}

	~Mouse() = default;

	friend class Input;
};
