#pragma once
#include "BaseTemplate.h"
#include "Time.h"

//template <typename T>
class Button 
{
public:

    std::chrono::system_clock::time_point start;
    std::wstring ButtonLabel;
    //T* ptr;

    bool started = false;
    D2D1_RECT_F rect = { 40,40,330,330 };
    ID2D1Brush* brush;
    IDWriteTextFormat* Format;
    IDWriteTextFormat* ShrinkedFormat;
    ID2D1Brush* brushText;
    DWRITE_TEXT_ALIGNMENT alignment;
    bool ShouldAnimate = false;
    int style;
    ID2D1HwndRenderTarget* ptr;

    HRESULT drawText(IDWriteTextFormat* Format, std::wstring str, D2D1_RECT_F rect, ID2D1Brush* brush, DWRITE_TEXT_ALIGNMENT alignment)
    {
        Format->SetTextAlignment(alignment);
        Format->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
        ptr->DrawTextW((CONST WCHAR*)str.c_str(), lstrlenW((LPCWSTR)str.c_str()), Format, rect, brush);
        return S_OK;
    }

    Button(ID2D1HwndRenderTarget* ptr, ID2D1SolidColorBrush* brush, std::wstring ButtonLabel, IDWriteTextFormat* Format, IDWriteTextFormat* ShrinkedFormat, ID2D1SolidColorBrush* brushText, DWRITE_TEXT_ALIGNMENT alignment, int style)
    {
        this->ptr = ptr;
        this->brush = brush;
        this->ButtonLabel = ButtonLabel;
        this->Format = Format;
        this->ShrinkedFormat = ShrinkedFormat;
        this->brushText = brushText;
        this->alignment = alignment;
        this->style = style;
    }

    Button(Button& b) = delete;


    void DrawButton(float RadiusX, float RadiusY)
    {
        if (ShouldAnimate)
        {
            ShrinkButton();
        }

        switch (style)
        {
        case 0:
        {
            D2D1_ROUNDED_RECT r = MiscUtils::RectToRoundedRect(rect, RadiusX, RadiusY);
            ptr->/*pRenderTarget->*/FillRoundedRectangle(MiscUtils::RectToRoundedRect(rect, RadiusX, RadiusY), brush);
        }
        case 1:
        {
            ptr->/*pRenderTarget->*/DrawRoundedRectangle(MiscUtils::RectToRoundedRect(rect, RadiusX, RadiusY), brush, 1.1, 0);
        }
        }

        drawText(ShouldAnimate ? ShrinkedFormat : Format, ButtonLabel, rect, brushText, alignment);
    }

    void ShrinkButton()
    {
        rect.left += 20;
        rect.right -= 20;
        rect.top += 10;
        rect.bottom -= 10;
    }


    ~Button()
    {
        RtlZeroMemory(&rect, sizeof(rect));
        brush->Release();
        Format->Release();
    }
};

