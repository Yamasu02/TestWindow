#pragma once
#include "Direct3D.h"
#include "D3DBuffer.h"





class VertexShader 
{
public:

    Direct3D* ptr;
    ID3DBlob* errblob = nullptr;
    ID3DBlob* m_vsblob = nullptr;
    ID3D11VertexShader* m_vs = nullptr;


    VertexShader(Direct3D* ptr, LPCWSTR str) 
    {
        this->ptr = ptr;
        this->ptr->hr = D3DCompileFromFile(str, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "vs_5_0", NULL, NULL, &m_vsblob, &errblob);
        this->ptr->hr = this->ptr->device->CreateVertexShader(m_vsblob->GetBufferPointer(), m_vsblob->GetBufferSize(), nullptr, &m_vs);
    }

    void SetVertexShader()
    {
        this->ptr->context->VSSetShader(m_vs, nullptr, 0u);
    }
};