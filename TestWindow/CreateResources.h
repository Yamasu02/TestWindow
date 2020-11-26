#pragma once

#include <iostream>
#include <exception>


#include "D2Dheaders.h"
#include "Logger.h"

using namespace Microsoft::WRL;
class ConstructBase : public std::exception, public Logger
{
public:

    static inline RECT rc = { 0 };
    static inline FLOAT width = 0.f;
    static inline FLOAT height = 0.f;
    static inline HRESULT hr = E_FAIL;

    static inline Microsoft::WRL::ComPtr <ID2D1Factory> pDirect2dFactory = nullptr;
    static inline Microsoft::WRL::ComPtr <ID2D1HwndRenderTarget> pRenderTarget = nullptr;
    static inline Microsoft::WRL::ComPtr <IDWriteFactory> pDWriteFactory = nullptr;
    static inline Microsoft::WRL::ComPtr <IWICImagingFactory> pIWICFactory = nullptr;

    enum errors
    {
        BadHRESULT = 0
    };

    HRESULT CheckHRESULT(HRESULT hr, std::string Error)
    {
        this->hr = hr;
        try
        {
            if (!SUCCEEDED(hr))
            {
                WriteLine(Error.c_str(), false);
                throw BadHRESULT;
            }
        }
        catch (errors& i)
        {
            ExitProcess(EXIT_FAILURE);
        }
        return hr;
    }


    ConstructBase(HWND hwnd) : Logger("Logs")
    {
        GetClientRect(hwnd, &rc);
        D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

        CoInitialize(NULL);

        ConstructBase::CheckHRESULT(CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, &pIWICFactory), "WICFactory Error");
        ConstructBase::CheckHRESULT(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, pDirect2dFactory.GetAddressOf()), "D2D1Factory Error");
        ConstructBase::CheckHRESULT(pDirect2dFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_UNKNOWN)), D2D1::HwndRenderTargetProperties(hwnd, size, D2D1_PRESENT_OPTIONS_IMMEDIATELY), &pRenderTarget), "HwndTarget Error");
        ConstructBase::CheckHRESULT(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), &pDWriteFactory), "DWriteFactory Error");
        
     
    
    }


    //ConstructBase() = delete;


    HRESULT CreateSolidColorBrush(ID2D1SolidColorBrush** brush, D2D1_COLOR_F color)
    {
        return pRenderTarget->CreateSolidColorBrush(color, brush);
    }


    HRESULT CreateGradientColorBrush(D2D1_GRADIENT_STOP gradientStops[2], ID2D1LinearGradientBrush** pLinearGradientBrush)
    {
        ID2D1GradientStopCollection* pGradientStops;
        pRenderTarget->CreateGradientStopCollection(gradientStops, 2, D2D1_GAMMA_2_2, D2D1_EXTEND_MODE_CLAMP, &pGradientStops);
        return  pRenderTarget->CreateLinearGradientBrush(D2D1::LinearGradientBrushProperties(D2D1::Point2F(0, 0), D2D1::Point2F(550, 1100)), pGradientStops, pLinearGradientBrush);
    }

    
    HRESULT CreateFormat(std::string familyName, DWRITE_FONT_WEIGHT fontWeight, DWRITE_FONT_STYLE style, FLOAT size, IDWriteTextFormat** format)
    {
        return pDWriteFactory->CreateTextFormat((WCHAR*)&familyName, NULL, fontWeight, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, size, L"en-us", format);
    }


    HRESULT LoadBitmapFromResource(int id, LPCWSTR type, ID2D1Bitmap** ppBitmap)
    {
        IWICBitmapFrameDecode* pSource = nullptr;
        IWICStream* pStream = nullptr;
        IWICBitmapScaler* pScaler = nullptr;

        HRSRC imageResHandle = FindResource(NULL, MAKEINTRESOURCE(id), type);
        HRESULT hr = imageResHandle ? S_OK : E_FAIL;

        if (SUCCEEDED(hr))
        {
            HANDLE imageResDataHandle = LoadResource(NULL, imageResHandle);
            hr = imageResDataHandle ? S_OK : E_FAIL;

            if (SUCCEEDED(hr))
            {
                void* pImageFile = LockResource(imageResDataHandle);
                hr = pImageFile ? S_OK : E_FAIL;

                if (SUCCEEDED(hr))
                {
                    DWORD imageFileSize = SizeofResource(NULL, imageResHandle);
                    hr = imageFileSize ? S_OK : E_FAIL;

                    if (SUCCEEDED(hr))
                    {
                        hr = pIWICFactory->CreateStream(&pStream);
                    }

                    if (SUCCEEDED(hr))
                    {
                        hr = pStream->InitializeFromMemory((BYTE*)(pImageFile), imageFileSize);

                        if (SUCCEEDED(hr))
                        {
                            IWICBitmapDecoder* pDecoder = nullptr;
                            hr = pIWICFactory->CreateDecoderFromStream(pStream, NULL, WICDecodeMetadataCacheOnLoad, &pDecoder);
                            if (SUCCEEDED(hr))
                            {
                                hr = pDecoder->GetFrame(0, &pSource);
                                if (SUCCEEDED(hr))
                                {
                                    IWICFormatConverter* pConverter = nullptr;
                                    hr = pIWICFactory->CreateFormatConverter(&pConverter);

                                    if (SUCCEEDED(hr))
                                    {
                                        hr = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);


                                        if (SUCCEEDED(hr))
                                        {
                                            hr = pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, ppBitmap);
                                        }

                                        return hr;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }


    HRESULT LoadBitmapFromFile(CONST WCHAR* path, ID2D1Bitmap** ppBitmap)
    {
        IWICBitmapDecoder* pDecoder = nullptr;
        IWICBitmapFrameDecode* pSource = NULL;
        IWICStream* pStream = NULL;

        IWICBitmapScaler* pScaler = NULL;

        HRESULT hr = pIWICFactory->CreateDecoderFromFilename(path, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);


        if (SUCCEEDED(hr))
        {
            hr = pDecoder->GetFrame(0, &pSource);

            if (SUCCEEDED(hr))
            {
                IWICFormatConverter* pConverter = nullptr;
                hr = pIWICFactory->CreateFormatConverter(&pConverter);
                if (SUCCEEDED(hr))
                {
                    hr = pConverter->Initialize(pSource, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);

                    if (SUCCEEDED(hr))
                    {
                        hr = pRenderTarget->CreateBitmapFromWicBitmap(pConverter, NULL, ppBitmap);
                        return hr;
                    }
                }
            }
        }
    }
   

    BOOL UpdateBasicRsrces(HWND hWnd)
    {
        if (GetClientRect(hWnd, &rc))
        {
            D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);
            CheckHRESULT(pRenderTarget->Resize(size), "Failed on Resize");
            width = rc.right - rc.left;
            height = rc.bottom - rc.top;
            return true;
        }
        return false;
    }


    ~ConstructBase()
    {

    }
};

using namespace D2D1;
class CreateResources:public ConstructBase
{
public:


    struct Brushes
    {
        ID2D1SolidColorBrush* pWhiteBrush = NULL;
        ID2D1SolidColorBrush* pBlackBrush = NULL;
        ID2D1SolidColorBrush* pGrayBrush = NULL;
        ID2D1SolidColorBrush* pSemiOpaqueBrush = NULL;
        ID2D1LinearGradientBrush* pLinearGradientBrush = NULL;
        ID2D1LinearGradientBrush* pLinearGradientBrush2 = NULL;
        ID2D1LinearGradientBrush* pLinearGradientBrush3 = NULL;
    }Brushes;

    struct Colors
    {
        D3DCOLORVALUE White = { 1, 1, 1, 1 };
        D3DCOLORVALUE Black = { 0, 0, 0, 1 };
        D3DCOLORVALUE SemiOpaqueBlack = { 1, 1, 1, 0.1 };
        D3DCOLORVALUE PurpleBlack = { 0.117, 0.109, 0.129, 1 };
        D3DCOLORVALUE o = { 1, 1, 1, 0 };

    }Colors;

    struct GradientStops
    {
        D2D1_GRADIENT_STOP gradientStops[2] = { {0.1f,ColorF(ColorF::Black, 1)},{ 0.9, ColorF(ColorF::DarkSlateGray, 1)} };
        D2D1_GRADIENT_STOP gradientStops2[2] = { {0.1f, ColorF(ColorF::White, 1)},{ 0.25 ,ColorF(ColorF::Black, 1) } };
        D2D1_GRADIENT_STOP gradientStops3[2] = { {0.1f, ColorF(ColorF::DarkSlateGray, 1)}, { 0.25 ,ColorF(ColorF::Black, 0.9) } };
    }GS;

    struct WriteFormats
    {
        IDWriteTextFormat* pTextFormatNormal = nullptr;
        IDWriteTextFormat* pTextFormatSmaller = nullptr;
        IDWriteTextFormat* pTextFormatHugeItalic = nullptr;
        IDWriteTextFormat* pTextFormatHugeOblique = nullptr;
        IDWriteTextFormat* pTextFormatBigItalic = nullptr;
        IDWriteTextFormat* pTextFormatNewUser = nullptr;
        IDWriteTextFormat* pTextFormatSignIn = nullptr;
    }WriteFormats;

    struct Bitmaps
    {
        ID2D1Bitmap* pBitmapEmail = NULL;
        ID2D1Bitmap* pBitmapKey = NULL;
        ID2D1Bitmap* pBitmapLock = NULL;
        ID2D1Bitmap* pBitmapBgr = NULL;
        ID2D1Bitmap* pBitmapHs = NULL;
        ID2D1Bitmap* pBitmapVisual = NULL;
        ID2D1Bitmap* pBitmapAA = NULL;
        ID2D1Bitmap* pBitmapSkinChanger = NULL;
        ID2D1Bitmap* pBitmapSettings = NULL;
        ID2D1Bitmap* pBitmapAccount = NULL;
        ID2D1Bitmap* pBitmapFPS = NULL;
        ID2D1Bitmap* pBitmapLoading = NULL;
    }Bitmaps;

    struct Cursors
    {
        HCURSOR hCursorHand;
        HCURSOR hCursorArrow;
    }Cursors;

    struct DefualtBitmapRects
    {
        D2D1_RECT_F FPSBmpRect;
        D2D1_RECT_F FPSBmpTextRect;
    }DefaultBitmapRects;


    CreateResources(HWND hwnd) : ConstructBase(hwnd)
    {
        CreateBrushes();
        CreateFormats();
        LoadCursors();
    }


    void UpdateDefaultStuff()
    {
        DefaultBitmapRects.FPSBmpRect = { width * 1 / 40, height - 70, width * 3 / 40,height - 25 };
        DefaultBitmapRects.FPSBmpTextRect = { DefaultBitmapRects.FPSBmpRect.left,DefaultBitmapRects.FPSBmpRect.top + 30,DefaultBitmapRects.FPSBmpRect.right,DefaultBitmapRects.FPSBmpRect.bottom + 30 };
    }

    HRESULT CreateBrushes()
    {
        CheckHRESULT(CreateSolidColorBrush(&Brushes.pWhiteBrush, Colors.White), "WhiteBrush Error");
        CheckHRESULT(CreateSolidColorBrush(&Brushes.pBlackBrush, Colors.Black), "BlackBrush Error");
        CheckHRESULT(CreateSolidColorBrush(&Brushes.pGrayBrush, Colors.PurpleBlack), "BlackBrush Error");
        CheckHRESULT(CreateSolidColorBrush(&Brushes.pSemiOpaqueBrush, Colors.SemiOpaqueBlack), "BlackBrush Error");

        CheckHRESULT(CreateGradientColorBrush(GS.gradientStops, &Brushes.pLinearGradientBrush), "BlackBrush Error");
        CheckHRESULT(CreateGradientColorBrush(GS.gradientStops2, &Brushes.pLinearGradientBrush2), "BlackBrush Error");
        CheckHRESULT(CreateGradientColorBrush(GS.gradientStops3, &Brushes.pLinearGradientBrush3), "BlackBrush Error");

        return  hr;
    }

    HRESULT CreateFormats()
    {
        hr = CreateFormat("Italic", DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, 16.0f, &WriteFormats.pTextFormatNormal);
        hr = CreateFormat("Comic Sans", DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, 14.5f, &WriteFormats.pTextFormatSmaller);
        hr = CreateFormat("Comic Sans", DWRITE_FONT_WEIGHT_THIN, DWRITE_FONT_STYLE_ITALIC, 50.0f, &WriteFormats.pTextFormatHugeItalic);
        hr = CreateFormat("Comic Sans", DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_OBLIQUE, 50.5f, &WriteFormats.pTextFormatHugeOblique);
        hr = CreateFormat("Comic Sans", DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_ITALIC, 26.f, &WriteFormats.pTextFormatBigItalic);
        return hr;
    }


    void LoadCursors()
    {
        Cursors.hCursorHand = LoadCursor(0, MAKEINTRESOURCE(IDC_HAND));
        Cursors.hCursorArrow = LoadCursor(0, MAKEINTRESOURCE(IDC_ARROW));
    }
};
using namespace Microsoft::WRL;


