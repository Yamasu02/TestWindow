#pragma once
#include "BaseTemplate.h"

template <typename T>
class StaticSlider :public Box
{

public:

    bool state = false;
    ID2D1Brush* pBrush;
    D2D1_ELLIPSE ellipse;
    T* ptr;

    StaticSlider(T* ptr, ID2D1Brush* pBrush, D2D1_RECT_F rect)
    {
        this->ptr = ptr;
        this->rect_f = rect;
        this->pBrush = pBrush;
    }

    void DrawSlider()
    {
        ptr->pRenderTarget->DrawRoundedRectangle(ptr->RectToRoundedRect(rect_f, 20, 20), pBrush);
        ellipse = { {rect_f.left + (rect_f.right - rect_f.left) / 4,rect_f.top + (rect_f.bottom - rect_f.top) / 2}, (rect_f.bottom - rect_f.top) / 2, (rect_f.bottom - rect_f.top) / 2 };
        if (state)
        {
            ellipse.point = { rect_f.left + (rect_f.right - rect_f.left) * 3 / 4,rect_f.top + (rect_f.bottom - rect_f.top) / 2 };
        }
        ptr->pRenderTarget->FillEllipse(ellipse, pBrush);
    }
};
