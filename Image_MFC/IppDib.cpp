#include "pch.h"
#include "IppDib.h"


IppDib::IppDib()
    : m_nWidth(0), m_nHeight(0), m_nBitCount(0), m_nDibSize(0), m_pDib(NULL)
{
}
IppDib::IppDib(const IppDib& dib)
    : m_nWidth(dib.m_nWidth), m_nHeight(dib.m_nHeight), m_nBitCount(dib.m_nBitCount),
    m_nDibSize(dib.m_nDibSize), m_pDib(NULL)
{
    if (dib.m_pDib != NULL)
    {
        m_pDib = new BYTE[m_nDibSize];
        memcpy(m_pDib, dib.m_pDib, m_nDibSize);
    }
}

IppDib::~IppDib()
{
    if (m_pDib)
        delete[] m_pDib;
}

BOOL IppDib::CreateGrayBitmap(LONG nWidth, LONG nHeight)
{
    if (m_pDib)
        Destroy();

    m_nWidth = nWidth;
    m_nHeight = nHeight;
    m_nBitCount = 8;

    DWORD dwWidthStep = (m_nWidth * m_nBitCount / 8 + 3) & ~3;
    DWORD dwSizeImage = (m_nHeight * dwWidthStep);
    m_nDibSize = sizeof(BITMAPINFOHEADER) + (sizeof(RGBQUAD) * GetPaletteNums()) + dwSizeImage;

    m_pDib = new BYTE[m_nDibSize];
    if (m_pDib == NULL)
        return FALSE;

    // BITMAPINFOHEADER ����ü ����
    LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
    lpbmi->biSize = sizeof(BITMAPINFOHEADER);
    lpbmi->biWidth = m_nWidth;
    lpbmi->biHeight = m_nHeight;
    lpbmi->biPlanes = 1;
    lpbmi->biBitCount = m_nBitCount;
    lpbmi->biCompression = BI_RGB;
    lpbmi->biSizeImage = dwSizeImage;
    lpbmi->biXPelsPerMeter = 0;
    lpbmi->biYPelsPerMeter = 0;
    lpbmi->biClrUsed = 0;
    lpbmi->biClrImportant = 0;

    // �׷��̽����� �ȷ�Ʈ ����
    RGBQUAD* pPal = (RGBQUAD*)((BYTE*)m_pDib + sizeof(BITMAPINFOHEADER));
    for (int i = 0; i < 256; i++)
    {
        pPal->rgbBlue = (BYTE)i;
        pPal->rgbGreen = (BYTE)i;
        pPal->rgbRed = (BYTE)i;
        pPal->rgbReserved = 0;
        pPal++;
    }

    // �ȼ� ������ �ʱ�ȭ
    BYTE* pData = GetDIBitsAddr();
    memset(pData, 0, dwSizeImage);

    return TRUE;
}

BOOL IppDib::CreateRgbBitmap(LONG nWidth, LONG nHeight)
{
    if (m_pDib)
        Destroy();

    m_nWidth = nWidth;
    m_nHeight = nHeight;
    m_nBitCount = 24;

    DWORD dwWidthStep = (m_nWidth * m_nBitCount / 8 + 3) & ~3;
    DWORD dwSizeImage = (m_nHeight * dwWidthStep);
    m_nDibSize = sizeof(BITMAPINFOHEADER) + dwSizeImage;

    m_pDib = new BYTE[m_nDibSize];
    if (m_pDib == NULL)
        return FALSE;

    // BITMAPINFOHEADER ����ü ����
    LPBITMAPINFOHEADER lpbmi = (LPBITMAPINFOHEADER)m_pDib;
    lpbmi->biSize = sizeof(BITMAPINFOHEADER);
    lpbmi->biWidth = m_nWidth;
    lpbmi->biHeight = m_nHeight;
    lpbmi->biPlanes = 1;
    lpbmi->biBitCount = m_nBitCount;
    lpbmi->biCompression = BI_RGB;
    lpbmi->biSizeImage = dwSizeImage;
    lpbmi->biXPelsPerMeter = 0;
    lpbmi->biYPelsPerMeter = 0;
    lpbmi->biClrUsed = 0;
    lpbmi->biClrImportant = 0;

    // �ȼ� ������ �ʱ�ȭ
    BYTE* pData = GetDIBitsAddr();
    memset(pData, 0, dwSizeImage);

    return TRUE;
}

void IppDib::Destroy()
{
    if (m_pDib)
    {
        delete[] m_pDib;
        m_pDib = NULL;
    }

    m_nWidth = 0;
    m_nHeight = 0;
    m_nBitCount = 0;
    m_nDibSize = 0;
}