#pragma once
#include "BaseTemplate.h"

template <typename T>
class Slider : public Box
{

public:

    int position;
    bool Active;
    ID2D1Brush* pBrush;
    D2D1_ELLIPSE ellipse;
    T* ptr;
    POINT k;

    Slider(T* ptr, ID2D1Brush* pBrush, D2D1_RECT_F rect)
    {
        this->ptr = ptr;
        this->rect_f = rect;
        this->pBrush = pBrush;
        ellipse = { {rect_f.left + (rect_f.right - rect_f.left) / 4,rect_f.top + (rect_f.bottom - rect_f.top) / 2}, (rect_f.bottom - rect_f.top) / 2, (rect_f.bottom - rect_f.top) / 2 };
    }


    void UpdateEllipse(HWND hWnd)
    {
        if (GetAsyncKeyState(VK_LBUTTON) && (ptr->CursorBelongsToRect(hWnd, rect_f)))
        {
            position = ptr->Points.point.x - 100;
            SetWindowText(hWnd, (LPCWSTR)&std::to_wstring(position));
        }
        ellipse.point = { rect_f.left + position,rect_f.top + (rect_f.bottom - rect_f.top) / 2 };
    }


    void DrawSlider(HWND hwnd)
    {
        UpdateEllipse(hwnd);
        ptr->pRenderTarget->FillEllipse(ellipse, pBrush);
        ptr->pRenderTarget->DrawRoundedRectangle(ptr->RectToRoundedRect(rect_f, 25, 25), pBrush);
    }
};