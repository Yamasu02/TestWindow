#include "ColorPicker.h"


ColorPicker::ColorPicker() : Window(L"", (WNDPROC)WinProc, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, 0, 700, 400)
{
	//function ptr to call non-static OnDraw function from static WinProc
	OnDrawPtr = std::bind(&ColorPicker::OnDraw, this);
	//Set some non-static variables as window properties for calling them inside WinProc(static function)
	SetProp(hwnd, L"this", this);
	SetProp(hwnd, L"OnDraw", &OnDrawPtr);

	//CreateInputInterfaces
	mouse = new Mouse(hwnd);
	kb = new KeyBoard();


	//Setting Brushes paint areas
	LinearGradientBrush.rect = { c->width * 0.25f, 0, c->width, c->height * 0.41f };
	LinearGradientBlackBrush.rect = { c->width * 0.25f, 0, c->width, c->height * 0.41f };
	SolidColorBrush.rect = { 0,0,c->width * 0.25f ,c->height * 0.41f };
	LinearGradientBrushRgb.rect = { c->width * 1 / 20, c->height / 2, c->width * 19 / 20, c->height / 2 + 15 };
	//DefaultBrushColors
	SolidColorBrush.color = { 1,0,0,1 };
	LinearGradientBrush.color = { 1,0,0,1 };
	//CreateBrushes
	c->CreateLinearGradientBrush(&LinearGradientBlackBrush.pBrush, { 0,0,0,0 }, { 0,0,0,1 }, { 0, 0 }, { 0, 250 }, 0.f, 1.f);
	c->CreateLinearGradientBrush(&LinearGradientBrush.pBrush, { 1,1,1,1 }, { 1,0,0,1 }, { 200, 0 }, { 800, 250 }, 0.f, 1.f);
	c->CreateSolidColorBrush(&SolidColorBrush.pBrush, { 1,0,0,1 });
	//CreateMultiColorSliderbrush
	D2D1_COLOR_F col[] = { ColorF(0xff0000, 1) ,ColorF(0xffff00, 1) ,ColorF(0x00ff00, 1) ,ColorF(0x00ffff, 1),ColorF(0x0000ff, 1),ColorF(0xff00ff, 1),ColorF(0xff0000, 1) };
	float offsets[] = { 0.08f , 0.28f ,0.42f ,0.5 ,0.70f ,0.84 ,1.f };
	c->CreateLinearGradientBrush(&LinearGradientBrushRgb.pBrush, col, ARRAYSIZE(col), MiscUtils::GetUpperLeftCorner(LinearGradientBrushRgb.rect), MiscUtils::GetLowerRightCorner(LinearGradientBrushRgb.rect), offsets);


	//CreateCursors
	hCurCross = LoadCursor(0, IDC_CROSS); hCurArrow = LoadCursor(0, IDC_ARROW);


	//CreateTextBoxes
	TBoxes[0] = new TextBox(c, { c->width * 1 / 20, c->height * 67 / 100,c->width * 19 / 20,c->height * 76 / 100 }, { 0,0,0,1 }, {0,0,0,1}, nullptr, L"", L"Hexadecimal", 7);
	TBoxes[1] = new TextBox(c, { c->width * 1/20 ,c->height * 86/100,c->width * 9/40 ,c->height * 96/100 }, { 0,0,0,1 }, { 0,0,0,1 }, nullptr, L"", L"RGB",11);
	TBoxes[2] = new TextBox(c, { c->width * 11/40,c->height * 86/100,c->width * 19/40,c->height * 96/100 }, { 0,0,0,1 }, { 0,0,0,1 }, nullptr, L"", L"CMYK",20);
	TBoxes[3] = new TextBox(c, { c->width * 21/40,c->height * 86/100,c->width * 29/40,c->height * 96/100 }, { 0,0,0,1 }, { 0,0,0,1 }, nullptr, L"SoonTM", L"HSV",20);
	TBoxes[4] = new TextBox(c, { c->width * 31/40,c->height * 86/100,c->width * 39/40,c->height * 96/100 }, { 0,0,0,1 }, { 0,0,0,1 }, nullptr, L"SoonTM", L"HSL",20);


	//CreateSliders
	pSlider = new Slider(c, LinearGradientBrushRgb.rect, 5, 5, LinearGradientBrushRgb.pBrush,nullptr, 15);

	for (auto& i : TBoxes) i->D2D = c;
}


void ColorPicker::OnUpdate()
{
	mouse->OnUpdate();
	if (mouse->ButtonStates.find(L"LeftClick")->second)  OnClick();
	//Update TextBoxes
	for (auto& CurrTBox : TBoxes)  CurrTBox->OnUpdate();
	//Update Strings
	if (!ActiveTextBox)
	{
		TBoxes[0]->Text.first = L"0x" + ColorToText::ColorToHexStr(SolidColorBrush.color);
		TBoxes[2]->Text.first = ColorToText::RgbToCMYK(SolidColorBrush.color);
		TBoxes[1]->Text.first = ColorToText::RgbToStringRgb(SolidColorBrush.color);
	}
}
void ColorPicker::OnDraw()
{
	c->pRenderTarget->BeginDraw();
	c->pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::WhiteSmoke));

	c->pRenderTarget->FillRectangle(LinearGradientBrush.rect, LinearGradientBrush.pBrush);
	c->pRenderTarget->FillRectangle(LinearGradientBlackBrush.rect, LinearGradientBlackBrush.pBrush);
	c->pRenderTarget->FillRectangle(SolidColorBrush.rect, SolidColorBrush.pBrush);

	pSlider->OnDraw();

	for (auto& CurrTBox : TBoxes)  CurrTBox->OnDraw();

	c->pRenderTarget->EndDraw();
}
void ColorPicker::OnClick()
{
	//if click was on a TextBox
	for (auto& CurrTBox : TBoxes)
	{
		if (mouse->CursorBelongsToRect(CurrTBox->Rects.BoxRect))
		{
			//Make the clicked box active
			CurrTBox->OnFocus();
			ActiveTextBox = CurrTBox;
			//Make the rest inactive
			for (auto& CurrTBox : TBoxes)
			{
				if (CurrTBox != ActiveTextBox) CurrTBox->OnLoseFocus();
			}
			break;
		}
		else
			ActiveTextBox = nullptr;
	}
	//if click was in no textboxes
	if(ActiveTextBox == nullptr)
		for (auto& CurrTBox : TBoxes) CurrTBox->OnLoseFocus();

	//if click was on Gradient
	if (mouse->CursorBelongsToRect(LinearGradientBrush.rect))
	{
		SolidColorBrush.color = MiscUtils::ColorRefToRgb(GetPixel(hdc, mouse->WindowPos.x, mouse->WindowPos.y));// Solidcolor = clicked pixel
		c->pRenderTarget->CreateSolidColorBrush(SolidColorBrush.color, &SolidColorBrush.pBrush);
		SetCursor(hCurCross);
	}
	//if click was on Slider
	else if (mouse->CursorBelongsToRect(pSlider->SliderRectRange))
	{
		pSlider->OnClick(mouse->WindowPos.x);
		LinearGradientBrush.color = MiscUtils::ColorRefToRgb(GetPixel(hdc, pSlider->ellipse.point.x, pSlider->ellipse.point.y));//Main gradient'scolor = pixel clicked
		c->CreateLinearGradientBrush(&LinearGradientBrush.pBrush, { 1,1,1,1 }, LinearGradientBrush.color, { LinearGradientBrush.rect.left },
			{ LinearGradientBrush.rect.right, LinearGradientBrush.rect.bottom }, 0.f, 1.f);
		c->CreateLinearGradientBrush(&LinearGradientBlackBrush.pBrush, { 0,0,0,0 }, { 0,0,0,1 }, { 0,0 }, { 0, LinearGradientBlackBrush.rect.bottom }, 0.f, 1.f);
		SetCursor(hCurArrow);
	}
}


LRESULT ColorPicker::WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
	switch (Msg)
	{
		case  WM_LBUTTONDOWN:
			GetThis(hWnd)->mouse->ButtonStates.find(L"LeftClick")->second = true;
			break;

		case WM_LBUTTONUP:
			GetThis(hWnd)->mouse->ButtonStates.find(L"LeftClick")->second = false;
			break;

		case WM_MOUSEMOVE:
			GetThis(hWnd)->mouse->WindowPos = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			break;

		case WM_CHAR:
			GetThis(hWnd)->kb->KeyPressed.KeyCode = static_cast<std::byte>(wParam);
			GetThis(hWnd)->kb->KeyPressed.lParam = (std::bitset<32>)lParam; 
			if (GetThis(hWnd)->ActiveTextBox != nullptr)
			{
				switch (GetThis(hWnd)->kb->KeyPressed.KeyCode)
				{
					case (std::byte)VK_BACK:
						GetThis(hWnd)->ActiveTextBox->RemoveChar();

					default:
						GetThis(hWnd)->ActiveTextBox->PushChar((wchar_t)GetThis(hWnd)->kb->KeyPressed.KeyCode);
				}
			}
			break;

		case WM_PAINT:

			(*GetOnDraw(hWnd))();
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hWnd, Msg, wParam, lParam);
	}
}


void ColorPicker::ProceedMsg()
{
	OnUpdate();
	PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);

	if (msg.message == WM_QUIT) ExitProcess(EXIT_SUCCESS);

	TranslateMessage(&msg);
	DispatchMessage(&msg);
	UpdateWindow(hwnd);
	Sleep(2);
}
