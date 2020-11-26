#pragma once

#include <d2d1.h>

class MiscUtils
{
public:

    static D2D1_RECT_F RectToD2DRect(RECT rect)
    {
        return { (FLOAT)rect.left , (FLOAT)rect.top , (FLOAT)rect.right , (FLOAT)rect.bottom };
    }

    static D2D1_ROUNDED_RECT RectToRoundedRect(D2D1_RECT_F rect, float x, float y)
    {
        return { (FLOAT)rect.left , (FLOAT)rect.top , (FLOAT)rect.right , (FLOAT)rect.bottom };
    }

    static BOOL PointBelongsToRect(HWND hwnd, LPPOINT lp, D2D1_RECT_F rect)
    {
        if (lp->x >= rect.left && lp->x <= rect.right && lp->y >= rect.top && lp->y <= rect.bottom)
        {
            return true;
        }
        else
        {
            return false;
        }
        return false;
    }


    static BOOL ScreenPointBelongsToRect(HWND hwnd, LPPOINT lp, D2D1_RECT_F rect)
    {
        if (ScreenToClient(hwnd, lp))
        {
            if (PointBelongsToRect(hwnd, lp, rect))
            {
                return true;
            }
        }
        return false;
    }
};