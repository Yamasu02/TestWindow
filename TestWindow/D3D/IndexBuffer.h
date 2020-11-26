#pragma once
#include "D3DBuffer.h"

class IndexBuffer : public D3DBuffer
{
public:

    HRESULT hr;
    Direct3D* ptr;

    UINT offset = 0;
    UINT size_list = 0;


    IndexBuffer(Direct3D* ptr, void* list_indices, UINT size_list)
    {
        this->ptr = ptr;
        this->size_list = size_list;
        desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0u;
        desc.ByteWidth = 4 * size_list;

        sd.pSysMem = list_indices;
        hr = this->ptr->device->CreateBuffer(&desc, &sd, &pBuff);   
    }

    void SetIndexBuffer()
    {
        ptr->context->IASetIndexBuffer(pBuff, DXGI_FORMAT_R32_UINT, 0);
    }

};

