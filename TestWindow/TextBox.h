#pragma once
#include "BaseTemplate.h"

template <typename T>
class TextBox : public Box, Time
{
public:

    T* ptr;
    std::chrono::system_clock::time_point start;
    std::wstring text;
    std::wstring DefaultText;
    ID2D1Brush* pBrushBox;
    ID2D1Brush* pBrushText;
    IDWriteTextFormat* pTextFormat;
    DWRITE_TEXT_ALIGNMENT alignment;
    D2D_RECT_F RectText;
    bool IsActive;

    TextBox(T* ptr, ID2D1Brush* brushBox, ID2D1Brush* brushText, std::wstring str, std::wstring DefaultStr, IDWriteTextFormat* pTextFormat, DWRITE_TEXT_ALIGNMENT alignment)
    {
        this->ptr = ptr;
        this->pBrushBox = brushBox;
        this->pBrushText = brushText;
        this->pTextFormat = pTextFormat;
        this->text = str;
        this->alignment = alignment;
        this->DefaultText = DefaultStr;
        this->RectText = { rect_f.left + 10,rect_f.top,rect_f.right,rect_f.bottom };

    }


    void DrawInput()
    {
        if (!text.empty())
        {
            ptr->drawText(pTextFormat, text, RectText, pBrushText, alignment);
        }
        if (!IsActive && text.empty())
        {
            ptr->drawText(pTextFormat, DefaultText, RectText, ptr->Brushes.pSemiOpaqueBrush, alignment);
        }
    }


    void DrawBox(float RadiusX, float RadiusY)
    {
        ptr->pRenderTarget->DrawRoundedRectangle(ptr->RectToRoundedRect(rect_f, RadiusX, RadiusY), pBrushBox, 1);
        DrawInput();
    }


    void DrawCursor()
    {
        ptr->pRenderTarget->DrawLine({ rect_f.left + 10, rect_f.top + 3 }, { rect_f.left + 10,rect_f.bottom - 3 }, pBrushText, 1, NULL);
    }


    void ClearString()
    {
        text.clear();
    }
};