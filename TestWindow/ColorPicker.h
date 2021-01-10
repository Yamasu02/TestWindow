class ColorPicker : Window
{
public:

	KeyBoard* kb;
	Mouse* mouse;


	Direct2D::Brush<ID2D1LinearGradientBrush*> LinearGradientBrushRgb;
	Direct2D::Brush<ID2D1LinearGradientBrush*> LinearGradientBrush;
	Direct2D::Brush<ID2D1LinearGradientBrush*> LinearGradientBlackBrush;
	Direct2D::Brush<ID2D1SolidColorBrush*>	   SolidColorBrush;


	std::array<TextBox*, 5> TBoxes; 
	TextBox* ActiveTextBox;
	Slider* pSlider;


	HCURSOR hCurCross,hCurArrow;
	std::function<void(void)> OnDrawPtr;

	//functions to use non-static variables in WinProc(static)
	static inline ColorPicker* GetThis(HWND hwnd) { return (ColorPicker*)GetProp(hwnd, L"this"); }
	static inline std::function<void(void)>* GetOnDraw(HWND hwnd) { return (std::function<void(void)>*)GetProp(hwnd, L"OnDraw"); }
	

	ColorPicker();

	void OnClick();
	void OnDraw();
	void OnUpdate() override;

	static LRESULT WinProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
	void ProceedMsg() override;
};
