#pragma once


class D3DBuffer 
{
private:

    HRESULT hr;
    ID3D11Buffer* pBuff;
    D3D11_BUFFER_DESC desc;
    D3D11_SUBRESOURCE_DATA sd;

    D3DBuffer() : hr(0), pBuff(nullptr), desc({0}), sd({0})
    {
        
    }

    HRESULT CreateBuffer(ID3D11Device* device)
    {
        return device->CreateBuffer(&desc, &sd, &pBuff);
    }

    friend class ConstantBuffer;
    friend class VertexBuffer;
    friend class VertexShader;
    friend class IndexBuffer;
};

