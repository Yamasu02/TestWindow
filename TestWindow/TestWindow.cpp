#pragma once
#include <Windows.h>
#include "Input.h"
#include "Window.h"




int CALLBACK  WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdSHow)
{
	Window window(L"Class", (WNDPROC)Window::WinProc);

	/*{
		Logger log("of.txt",false);
		log.WriteLine("new line ffs\n", true);
		log.FlushBuffer();
		auto xt = log.ReadAllContent();
		auto xd = log.ReadLine();
		auto xd2 = log.ReadLine(2);
		auto xd3 = log.ReadLine(3);
	}*/


	
	window.MsgLoop();
}
