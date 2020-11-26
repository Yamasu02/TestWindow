#pragma once
#include "D3DBuffer.h"
#include "Direct3D.h"


class VertexBuffer : public D3DBuffer
{
public:

    HRESULT hr;
    Direct3D* ptr;

    ID3D11InputLayout* LayOut;
    UINT size = 0;
    UINT offset = 0;


    VertexBuffer(Direct3D* ptr,const void* Vertices, SIZE_T StructSize, UINT ElementsNum, const D3D11_INPUT_ELEMENT_DESC* ied, UINT iedSize, void* VsBlobPtr, SIZE_T VsBlobSize)
    {
        this->ptr = ptr;
        desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        desc.Usage = D3D11_USAGE_DEFAULT;
        desc.CPUAccessFlags = 0;
        desc.MiscFlags = 0u;
        desc.ByteWidth = StructSize * ElementsNum;
        desc.StructureByteStride = StructSize;
        sd.pSysMem = Vertices;
        hr = this->ptr->device->CreateBuffer(&desc, &sd, &pBuff);
        this->size = StructSize;
        hr = CreateInputLayout(ied, iedSize, VsBlobPtr, VsBlobSize);
    }

    void SetBuffer()
    {
        ptr->context->IASetVertexBuffers(0u, 1u, &pBuff, &size, &offset);
    }

    HRESULT CreateInputLayout(const D3D11_INPUT_ELEMENT_DESC* ied, UINT iedSize, void* VsBlobPtr, SIZE_T VsBlobSize)
    {
        return ptr->device->CreateInputLayout(ied, iedSize, VsBlobPtr, VsBlobSize, &LayOut);
    }

    void SetInputLayout()
    {
        ptr->context->IASetInputLayout(LayOut);
    }

};
