
#include "Direct3D.h"
#include "ViewPort.h"
#include "VertexBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"

Direct3D::Direct3D(HWND hWnd)
{
        desc.BufferDesc.Width = 1920;
        desc.BufferDesc.Height = 1080;
        desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        desc.BufferDesc.RefreshRate.Numerator = 60;
        desc.BufferDesc.RefreshRate.Denominator = 1;
        //desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        //desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        desc.SampleDesc.Count = 4;
        desc.SampleDesc.Quality = 0;
        desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        desc.BufferCount = 1;
        desc.OutputWindow = hWnd;
        desc.Windowed = TRUE;
        //desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
        desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
        D3D_FEATURE_LEVEL FeatureLevels[] = { D3D_FEATURE_LEVEL_11_0 };

        HRESULT hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG,
            FeatureLevels, (UINT)ARRAYSIZE(FeatureLevels), D3D11_SDK_VERSION, &desc, &SwapChain, &device, NULL, &context);

        hr = SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
        hr = device->CreateRenderTargetView(pBackBuffer, nullptr, &view);
        context->OMSetRenderTargets(1u, &view, nullptr);


        cc.m_time = 0;

   
        

        D3D11_INPUT_ELEMENT_DESC ied[] =
        {
            {"POSITION", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,D3D11_INPUT_PER_VERTEX_DATA ,0},
            //{"POSITION", 1,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,D3D11_INPUT_PER_VERTEX_DATA ,0 },
            {"COLOR", 0,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 12,D3D11_INPUT_PER_VERTEX_DATA ,0 },
            {"COLOR", 1,  DXGI_FORMAT_R32G32B32_FLOAT, 0, 24,D3D11_INPUT_PER_VERTEX_DATA ,0 }
        };



        unsigned int index_list[] =
        {
            //FRONT SIDE
            0,1,2,  //FIRST TRIANGLE
            2,3,0,  //SECOND TRIANGLE
            //BACK SIDE
            4,5,6,
            6,7,4,
            //TOP SIDE
            1,6,5,
            5,2,1,
            //BOTTOM SIDE
            7,0,3,
            3,4,7,
            //RIGHT SIDE
            3,2,5,
            5,4,3,
            //LEFT SIDE
            7,6,1,
            1,0,7
        };


        Vertex Ver[] =
        {
            //FRONT FACE
            {Vector3D(-0.5f,-0.5f,-0.5f),    Vector3D(1,0,0),  Vector3D(0.2f,0,0) },
            {Vector3D(-0.5f,0.5f,-0.5f),    Vector3D(1,1,0), Vector3D(0.2f,0.2f,0) },
            { Vector3D(0.5f,0.5f,-0.5f),   Vector3D(1,1,0),  Vector3D(0.2f,0.2f,0) },
            { Vector3D(0.5f,-0.5f,-0.5f),     Vector3D(1,0,0), Vector3D(0.2f,0,0) },

            //BACK FACE
            { Vector3D(0.5f,-0.5f,0.5f),    Vector3D(0,1,0), Vector3D(0,0.2f,0) },
            { Vector3D(0.5f,0.5f,0.5f),    Vector3D(0,1,1), Vector3D(0,0.2f,0.2f) },
            { Vector3D(-0.5f,0.5f,0.5f),   Vector3D(0,1,1),  Vector3D(0,0.2f,0.2f) },
            { Vector3D(-0.5f,-0.5f,0.5f),     Vector3D(0,1,0), Vector3D(0,0.2f,0) }

        };

        vp = new ViewPort(1920, 1080, this);
        vp->SetViewPort();
        auto oof = ARRAYSIZE(index_list);
        ib = new IndexBuffer(this, index_list, ARRAYSIZE(index_list));
        ib->SetIndexBuffer();

        vs = new VertexShader(this, L"C:\\Users\\Supreme1337\\source\\repos\\Direct3D\\Direct3D\\VertexShader.hlsl");
        ps = new PixelShader(this, L"C:\\Users\\Supreme1337\\source\\repos\\Direct3D\\Direct3D\\PixelShader.hlsl");



        buffer = new VertexBuffer(this, Ver, sizeof(Vertex), ARRAYSIZE(Ver), ied, ARRAYSIZE(ied), vs->m_vsblob->GetBufferPointer(), vs->m_vsblob->GetBufferSize());

        //buffer->SetBuffer();
        //buffer->SetInputLayout();


        cb = new ConstantBuffer();
        cb->Load(this, &cc, sizeof(constant));
}

void Direct3D::UpdateQuadPos(HWND hwnd)
{
    constant cc;
    cc.m_time = ::GetTickCount();

    m_delta_pos += m_delta_time / 10.0f;
    if (m_delta_pos > 1.0f)
        m_delta_pos = 0;


    Matrix4x4 temp;
    m_delta_scale += m_delta_time / 0.55f;
   // m_delta_scale += m_delta_time / 0.15f;

    //cc.m_world.setScale(Vector3D::lerp(Vector3D(0.5, 0.5, 0), Vector3D(1.0f, 1.0f, 0), (sin(m_delta_scale) + 1.0f) / 2.0f));
    //cc.m_world.setScale(Vector3D(1, 1, 0));
    //temp.setTranslation(Vector3D::lerp(Vector3D(-1.5f, -1.5f, 0), Vector3D(1.5f, 1.5f, 0), m_delta_pos));
    if (GetAsyncKeyState(VK_TAB))
    {
        ikr += 0.01;
    }
    //temp.setTranslation((Vector3D(ikr, ikr, 0)));

    //cc.m_world *= temp;
    cc.m_world.setScale(Vector3D(1, 1, 1));

    temp.setIdentity();
    temp.setRotationZ(m_delta_scale);
    cc.m_world *= temp;

    temp.setIdentity();
    temp.setRotationY(m_delta_scale);
    cc.m_world *= temp;

    temp.setIdentity();
    temp.setRotationX(m_delta_scale);
    cc.m_world *= temp;

    cc.m_view.setIdentity();
    RECT xd;
    GetClientRect(hwnd, &xd);
    cc.m_proj.setOrthoLH
    (
        (xd.right - xd.left) / 400.0f,
        (xd.bottom - xd.top) / 400.0f,
        -4.0f,
        4.0f
    );


    cb->Update( &cc);
}

   // m_device_context->DrawIndexed(index_count, start_index_location, start_vertex_index);

void Direct3D::DrawTriangle(HWND hwnd)
{
    Clear();

    UpdateQuadPos(hwnd);

    cb->SetConstantBufferVS();
    cb->SetConstantBufferPS();

    vs->SetVertexShader();
    ps->SetPixelShader();


    buffer->SetBuffer();
    buffer->SetInputLayout();
    ib->SetIndexBuffer();

  

    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); //D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP

    //context->Draw(4u, 0u);
    auto oof = ib->size_list;
    context->DrawIndexed(ib->size_list, 0, 0);
    SwapChain->Present(1, 0u);

    m_old_delta = m_new_delta;
    m_new_delta = ::GetTickCount();

    m_delta_time = (m_old_delta) ? ((m_new_delta - m_old_delta) / 1000.0f) : 0;
}

void Direct3D::Clear()
{
    const float color[] = { 1.0,0,1.0,1.0 };
    context->ClearRenderTargetView(view, color);
}