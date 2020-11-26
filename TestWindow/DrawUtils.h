#include "CreateResources.h"
#include "KeyBoard.h"
#include "MiscConversions.h"


class DrawUtils : public CreateResources, public KeyBoard //, public MiscUtils
{

public:

	LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
	LARGE_INTEGER Frequency;
	int Fps = -1;

	auto  GetTimeElapsed()
	{
		ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
		ElapsedMicroseconds.QuadPart *= 1000000;
		return ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;
	}


	DrawUtils(HWND hWnd) : CreateResources(hWnd)
	{
		QueryPerformanceFrequency(&Frequency);
	}


	HRESULT drawText(IDWriteTextFormat* Format, std::wstring str, D2D1_RECT_F rect, ID2D1Brush* brush, DWRITE_TEXT_ALIGNMENT alignment)
	{
		hr = Format->SetTextAlignment(alignment);
		hr = Format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		pRenderTarget->DrawTextW((CONST WCHAR*)str.c_str(), lstrlenW((LPCWSTR)str.c_str()), Format, rect, brush);
		return hr;
	}
	
	void SetFps()
	{
		if (Fps == -1)
		{
			QueryPerformanceCounter(&StartingTime);
			Fps = 0;
			return;
		}

		QueryPerformanceCounter(&EndingTime);
		Fps = GetTimeElapsed() / 1000 * 60;
		QueryPerformanceCounter(&StartingTime);
	}
};