#pragma once
#include "Direct3D.h"

class PixelShader
{
public:

    Direct3D* ptr;
    ID3DBlob* errblob = nullptr;
    ID3DBlob* m_psblob = nullptr;
    ID3D11PixelShader* m_ps = nullptr;

    PixelShader(Direct3D* ptr, LPCWSTR str)
    {
        this->ptr = ptr;
        this->ptr->hr = D3DCompileFromFile(str, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "ps_5_0", NULL, NULL, &m_psblob, &errblob);
        this->ptr->hr = this->ptr->device->CreatePixelShader(m_psblob->GetBufferPointer(), m_psblob->GetBufferSize(), nullptr, &m_ps);
    }

    void SetPixelShader()
    {
        ptr->context->PSSetShader(m_ps, nullptr, 0u); //pPixelShader.Get()
    }
};
