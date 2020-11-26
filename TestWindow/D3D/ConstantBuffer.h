#pragma once
#include "Direct3D.h"


class ConstantBuffer : D3DBuffer
{
public:
   
    Direct3D* ptr;

    void Load(Direct3D* ptr, void* buffer, UINT size_buffer)
    {
        this->ptr = ptr;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.ByteWidth = size_buffer;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0;

        sd.pSysMem = buffer;

        HRESULT hr = CreateBuffer(ptr->device);
    }

    void Update(void* buffer)
    {
        ptr->context->UpdateSubresource(this->pBuff, NULL, NULL, buffer, NULL, NULL);
    }

    void SetConstantBufferVS()
    {
        ptr->context->VSSetConstantBuffers(0, 1, &pBuff);
    }

    void SetConstantBufferPS()
    {
        ptr->context->PSSetConstantBuffers(0, 1, &pBuff);
    }
};