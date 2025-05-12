// PDF_Reader.cpp : Defines the entry point for the application.
//

#include "PDF_Reader.h"
#include "renderPDF.h"
#define MAX_LOADSTRING 100
#include <commctrl.h>
#include <commdlg.h>
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "Comdlg32.lib")
#include <vector>

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
int scrollPos = 0;                              // Tracks position of pdf
std::vector<HBITMAP> pageBitmaps;               // Vector to store bitmaps for each page
HBRUSH hBackgroundBrush;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
int GetTotalHeight();



void LoadPDFPages(FPDF_DOCUMENT doc) {
    pageBitmaps.clear();
    int numPages = FPDF_GetPageCount(doc);

    for (int i = 0; i < numPages; ++i) {
        FPDF_PAGE page = FPDF_LoadPage(doc, i);
        int width = (int)FPDF_GetPageWidth(page);
        int height = (int)FPDF_GetPageHeight(page);

        // Create a DIB section (device-independent bitmap)
        BITMAPINFO bmi = { 0 };
        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = width;
        bmi.bmiHeader.biHeight = -height; // top-down DIB
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        void* buffer = nullptr;
        HDC screenDC = GetDC(NULL);
        HBITMAP hBitmap = CreateDIBSection(screenDC, &bmi, DIB_RGB_COLORS, &buffer, NULL, 0);
        ReleaseDC(NULL, screenDC);

        if (hBitmap && buffer) {
            FPDF_BITMAP pdfBitmap = FPDFBitmap_CreateEx(width, height, FPDFBitmap_BGRA, buffer, width * 4);
            FPDFBitmap_FillRect(pdfBitmap, 0, 0, width, height, 0xFFFFFFFF);
            FPDF_RenderPageBitmap(pdfBitmap, page, 0, 0, width, height, 0, 0);
            FPDFBitmap_Destroy(pdfBitmap);

            pageBitmaps.push_back(hBitmap);
        }

        FPDF_ClosePage(page);
    }
}


// Scroll handling and rendering
void OnSize(HWND hWnd) {
    RECT rc;
    GetClientRect(hWnd, &rc);

    // Calculate total height of all pages combined
    int totalHeight = 0;
    for (auto hBitmap : pageBitmaps) {
        BITMAP bmp;
        GetObject(hBitmap, sizeof(BITMAP), &bmp);
        totalHeight += bmp.bmHeight;
    }

    // Set the scroll range based on total height of pages
    SetScrollRange(hWnd, SB_VERT, 0, totalHeight - rc.bottom, FALSE);
}



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
  

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PDFREADER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PDFREADER));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Fills details out about window
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    
    WNDCLASSEXW wcex;    
    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PDFREADER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = hBackgroundBrush;
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PDFREADER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
} 

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable
   hBackgroundBrush = CreateSolidBrush(RGB(52, 58, 64)); // #343a40

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
                            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);
   //HWND hTab = CreateWindowEx(0, WC_TABCONTROL, L"", WS_CHILD | WS_VISIBLE,
   //                         0, 0, 500, 500, hWnd, (HMENU)IDC_MAIN_TAB, hInst, nullptr);
   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
    {
        int wmId = LOWORD(wParam);
        // Parse the menu selections:
        switch (wmId)
        {
        case IDM_ABOUT:
            DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
            break;

        case IDM_EXIT:
            DestroyWindow(hWnd);
            freePDFium();
            DeleteObject(hBackgroundBrush);
            break;

        case IDM_OPEN_PDF:
        {
            OPENFILENAME ofn = { 0 };
            WCHAR szFile[MAX_PATH] = { 0 };

            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFilter = L"PDF Files (*.pdf)\0*.pdf\0";
            ofn.lpstrFile = szFile;
            ofn.nMaxFile = MAX_PATH;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            if (GetOpenFileName(&ofn)) {
                char filePathA[MAX_PATH];
                WideCharToMultiByte(CP_ACP, 0, szFile, -1, filePathA, MAX_PATH, NULL, NULL);
                FPDF_DOCUMENT doc = initPDFium(filePathA);  // Make sure this returns a valid doc
                if (doc) {
                    LoadPDFPages(doc);                      // This must fill `pageBitmaps`
                    OnSize(hWnd);                           // Set scroll range
                    InvalidateRect(hWnd, NULL, TRUE);       // Force repaint
                }
                else {
                    MessageBox(hWnd, L"Failed to load PDF document.", L"Error", MB_ICONERROR);
                }
            }

        }
        break;

        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
    }
    break;


    case WM_SIZE:
    {
        OnSize(hWnd);  // Adjust scroll range when window is resized
    }
    break;

    case WM_VSCROLL:
    {
        int scrollEvent = LOWORD(wParam);
        int newPos = scrollPos;

        switch (scrollEvent) {
        case SB_LINEUP:
            newPos -= 10;  // Scroll up by 10 units
            break;
        case SB_LINEDOWN:
            newPos += 10;  // Scroll down by 10 units
            break;
        case SB_THUMBTRACK:
            newPos = HIWORD(wParam);  // Jump to the thumb position
            break;
        }

        RECT rcClient;
        GetClientRect(hWnd, &rcClient);

        // Clamp the new scroll position within valid bounds
        newPos = max(0, min(newPos, GetTotalHeight() - rcClient.bottom));

        if (newPos != scrollPos) {
            scrollPos = newPos;
            SetScrollPos(hWnd, SB_VERT, scrollPos, TRUE);
            InvalidateRect(hWnd, NULL, TRUE);  // Trigger repaint to update content
        }
        break;
    }
    case WM_MOUSEWHEEL:
    {
        int delta = GET_WHEEL_DELTA_WPARAM(wParam);  // Amount scrolled
        int scrollDelta = -delta / WHEEL_DELTA * 30;  // Adjust scroll speed (30 px per notch)

        RECT rcClient;
        GetClientRect(hWnd, &rcClient);
        int totalHeight = GetTotalHeight();

        // Update scroll position
        int newPos = scrollPos + scrollDelta;
        newPos = max(0, min(newPos, totalHeight - rcClient.bottom));

        if (newPos != scrollPos) {
            scrollPos = newPos;
            SetScrollPos(hWnd, SB_VERT, scrollPos, TRUE);
            InvalidateRect(hWnd, NULL, TRUE);  // Repaint with new position
        }

        return 0;
    }
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        RECT rcClient;
        GetClientRect(hWnd, &rcClient);

        // Fill the background with your custom color
        FillRect(hdc, &rcClient, hBackgroundBrush);

        int yPos = 0;

        for (auto hBitmap : pageBitmaps) {
            BITMAP bmp;
            GetObject(hBitmap, sizeof(BITMAP), &bmp);

            if (yPos + bmp.bmHeight >= scrollPos && yPos <= scrollPos + rcClient.bottom) {
                int xCentered = (rcClient.right - bmp.bmWidth) / 2;  // center horizontally

                HDC memDC = CreateCompatibleDC(hdc);
                SelectObject(memDC, hBitmap);
                BitBlt(hdc, xCentered, yPos - scrollPos, bmp.bmWidth, bmp.bmHeight, memDC, 0, 0, SRCCOPY);
                DeleteDC(memDC);
            }

            yPos += bmp.bmHeight;
        }

        EndPaint(hWnd, &ps);
    }
    break;

    case WM_DESTROY:
        // Clean up bitmaps and resources
        for (auto hBitmap : pageBitmaps) {
            DeleteObject(hBitmap);
        }
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

// Helper function to calculate the total height of all pages
int GetTotalHeight() {
    int totalHeight = 0;
    for (auto hBitmap : pageBitmaps) {
        BITMAP bmp;
        GetObject(hBitmap, sizeof(BITMAP), &bmp);
        totalHeight += bmp.bmHeight;
    }
    return totalHeight;
}

