#pragma once
#include "BaseTemplate.h"

template <typename T>
class CheckBox :public Box
{
public:

    T* ptr;
    ID2D1Brush* pBrush;
    D2D1_RECT_F PaintArea;
    bool Checked = true;
    int mode;

    CheckBox(T* ptr, D2D1_RECT_F rect, ID2D1Brush* pBrush, int mode)
    {
        this->ptr = ptr;
        this->rect_f = rect;
        this->pBrush = pBrush;
        this->mode = mode;
        this->PaintArea = rect;
        PaintArea.left += (rect_f.right - rect_f.left) / 7;
        PaintArea.right -= (rect_f.right - rect_f.left) / 7;
        PaintArea.top += (rect_f.bottom - rect_f.top) / 7;
        PaintArea.bottom -= (rect_f.bottom - rect_f.top) / 7;
    }


    void DrawCheckBox()
    {
        ptr->pRenderTarget->DrawRectangle(rect_f, pBrush);
        if (Checked)
        {
            switch (mode)
            {
            case 1:
            {
                ptr->pRenderTarget->FillRectangle(PaintArea, pBrush);
                break;
            }
            case 2:
            {
                ptr->pRenderTarget->DrawLine({ PaintArea.left,PaintArea.top + ((PaintArea.bottom - PaintArea.top) / 2) }, { PaintArea.left + ((PaintArea.right - PaintArea.left) / 2),PaintArea.bottom }, pBrush, 2);
                ptr->pRenderTarget->DrawLine({ PaintArea.left + ((PaintArea.right - PaintArea.left) / 2), PaintArea.bottom }, { PaintArea.right, PaintArea.top }, pBrush, 2);
                break;
            }
            }
        }
    }
};