#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")


#include <array>
#include "Matrix4x4.h"

class ViewPort;
class VertexShader;
class VertexBuffer;
class PixelShader;
class ConstantBuffer;
class IndexBuffer;


using namespace Microsoft::WRL;



class Direct3D
{

public:

    float ikr = 0.0f;
    ID3D11Device* device = nullptr;
    IDXGISwapChain* SwapChain = nullptr;
    DXGI_SWAP_CHAIN_DESC desc;
    ID3D11DeviceContext* context = nullptr;
    ID3D11Texture2D* pBackBuffer = nullptr;
    ID3D11RenderTargetView* view = nullptr;


    HRESULT hr;
    ViewPort* vp = nullptr;
    VertexBuffer* buffer = nullptr;
    VertexShader* vs = nullptr;
    PixelShader* ps = nullptr;
    ConstantBuffer* cb = nullptr;
    IndexBuffer* ib = nullptr;


    struct Vertex { Vector3D pos; Vector3D color; Vector3D color1; }; 


    __declspec(align(16))
    struct constant 
    { 
        Matrix4x4 m_world;
        Matrix4x4 m_view;
        Matrix4x4 m_proj;
        unsigned int  m_time; 
    };

    constant cc;

    Direct3D(HWND hWnd);

    void Clear();

    void UpdateQuadPos(HWND hwnd);

    void DrawTriangle(HWND hwnd);


    long m_old_delta;
    long m_new_delta;
    float m_delta_time;

    float m_delta_pos;
    float m_delta_scale;
    float m_delta_rot;
};