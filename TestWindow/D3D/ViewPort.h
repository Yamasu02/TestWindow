#pragma once
#include "Direct3D.h"

class ViewPort
{
public:

    Direct3D* ptr;
    D3D11_VIEWPORT vp;

    ViewPort(UINT width, UINT height, Direct3D* ptr)
    {
        this->ptr = ptr;
        vp.Width = width;
        vp.Height = height;
        //vp.MinDepth = 0;
       // vp.MaxDepth = 1;
        vp.TopLeftX = 0;
        vp.TopLeftY = 0;
    }

    void SetViewPort()
    {
        ptr->context->RSSetViewports(1u, &vp);
    }

};