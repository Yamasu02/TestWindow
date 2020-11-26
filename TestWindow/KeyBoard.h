#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <cstddef>
#include <bitset>

class KeyBoard
{
public:

	static bool IsKeyPressed(uint8_t Vk)
	{
		return GetKeyState(Vk);
	}

	static inline struct KeyPressed
	{
		std::byte KeyCode;
		std::bitset<32> lParam;
		uint8_t RepeatCount;
		uint8_t OEMcode;


		void UpdateRepeatCount()
		{
			std::memcpy((void*)&RepeatCount, &lParam, 2);
		}

		bool WasAltDown()
		{
			return lParam.test(29);
		}

		bool WasDownBefore()
		{
			return lParam.test(30);
		}

		void GetOEM()
		{
			std::memcpy((void*)(OEMcode), (void*)((&lParam) + 3), 1);
		}

		auto IsExtendedKey()
		{
			lParam.test(25);
		}

	}KeyPressed;

	INPUT input;

	void IntializeArtificialInputKb(uint8_t Vk)
	{
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = Vk;
		input.ki.wScan = MapVirtualKey(Vk, MAPVK_VK_TO_VSC);
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;
	}

	void KeyDown()
	{
		input.ki.dwFlags = 0;
		SendInput(1, &input, sizeof(INPUT));
	}

	void KeyUp()
	{
		input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(INPUT));
	}
};
