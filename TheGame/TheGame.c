// TheGame.cpp : Defines the entry point for the application.
//
#pragma comment(linker, "\"/manifestdependency:type='Win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "framework.h"
#include "TheGame.h"
#include <windows.h>
#include  "strsafe.h"


#define MAX_LOADSTRING 100
#define ID_HOTKEY 1
#define ID_HOTKEY2 2

#define IDM_FILE_NEW 1
#define IDM_FILE_OPEN 2
#define IDM_FILE_QUIT 3

typedef struct SPoint {
    float x, y;
} TPoint ;

TPoint point(float x, float y) {
    TPoint pt;
    pt.x = x;
    pt.y = y;
    return pt;
}

typedef struct SObject {
    TPoint pos;
    TPoint size;
    COLORREF brush;
    TPoint speed;
} TObject;

void ObjectInit(TObject* obj, float xPos, float yPos, float width, float height) {
    obj -> pos = point(xPos, yPos);
    obj->size = point(width, height);
    obj->brush = RGB(200, 0, 255);
    obj->speed = point(0,0);
}

void ObjectShow(TObject obj, HDC dc)
{
    SelectObject(dc, GetStockObject(DC_PEN));
    SetDCPenColor(dc, RGB(0, 0, 0));
    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, obj.brush);
    Rectangle(dc, (int)(obj.pos.x), (int)(obj.pos.y), 
        (int)(obj.pos.x + obj.size.x), (int)(obj.pos.y + obj.size.y));
}

void Move(TObject* obj)
{
    if (obj->pos.x + obj->speed.x > 0 && obj->pos.x + obj->speed.x < 1265 - obj->size.x)
    {
        obj->pos.x += obj->speed.x;
    }
    if (obj->pos.y + obj->speed.y > 0 && obj->pos.y + obj->speed.y < 665 - obj->size.y)
    {
        obj->pos.y += obj->speed.y;
    }
    

}

TObject player;
int playerSpeed = 5;

void Controls()
{
    player.speed.x = 0;
    player.speed.y = 0;
    if (GetKeyState('W') < 0) player.speed.y = -playerSpeed;
    if (GetKeyState('A') < 0) player.speed.x = -playerSpeed;
    if (GetKeyState('S') < 0) player.speed.y = playerSpeed;
    if (GetKeyState('D') < 0) player.speed.x = playerSpeed;
    if (player.speed.x != 0 && player.speed.y != 0)
    {
        player.speed = point(player.speed.x * 0.7, player.speed.y * 0.7);
    }

}


RECT rect;

void WinInitial()
{
    ObjectInit(&player, 100, 100, 100, 100);
}

void CharMove() {
    Controls();
    Move(&player); 

}

void Draw(HDC dc) {
    HDC memDC = CreateCompatibleDC(dc);
    HBITMAP memBM = CreateCompatibleBitmap(dc,rect.right- rect.left, rect.bottom - rect.top );
    SelectObject(memDC, memBM);
    SelectObject(memDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(memDC, RGB(255, 255, 255));
    Rectangle(memDC, 0, 0, 1280, 720);
    ObjectShow(player, memDC);
    
    BitBlt(dc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, memDC, 0, 0, SRCCOPY);
    DeleteDC(memDC);
    DeleteObject(memBM);

}

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void CenterWindow(HWND);
HWND hwndSta1;
HWND hwndSta2;
void CreateLabels(HWND);
void AddMenus(HWND);



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    MessageBoxW(NULL, L"TheBestGameEVER!!", L"First", MB_OK);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_THEGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
 /*   if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }*/
    HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
       20, 10, 1280, 720, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);


    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_THEGAME));

    MSG msg;
    HDC hdc = GetDC(hWnd);

    WinInitial();
    
    // Main message loop:
    while (1)
    {
       
        while (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
      
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (msg.message == WM_QUIT) {
            break;
        }
        CharMove();
        Draw(hdc);


    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;
    HBRUSH backgroundColor = CreateSolidBrush(RGB(100, 100, 100));

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_THEGAME));
    wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground  = backgroundColor;
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_THEGAME);
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

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

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
    wchar_t buf[10];
    switch (message)
    {
    case WM_CREATE:

        RegisterHotKey(hWnd, ID_HOTKEY, MOD_CONTROL, 0x43);
        RegisterHotKey(hWnd, ID_HOTKEY2, MOD_SHIFT, VK_F10);
        //CreateLabels(hWnd);

        AddMenus(hWnd);
        break;
    case WM_KEYDOWN:

        if (wParam == VK_ESCAPE) {

            int ret = MessageBoxW(hWnd, L"Are you sure to quit?",
                L"Message", MB_OKCANCEL);

            if (ret == IDOK) {

                SendMessage(hWnd, WM_CLOSE, 0, 0);
            }
        }

        break;
    case WM_COMMAND:

        switch (LOWORD(wParam)) {

        case IDM_FILE_NEW:
        case IDM_FILE_OPEN:

            MessageBeep(MB_ICONINFORMATION);
            break;

        case IDM_FILE_QUIT:

            SendMessage(hWnd, WM_CLOSE, 0, 0);
            break;
        case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
        }

        break;
    case WM_MOVE:

        GetWindowRect(hWnd, &rect);

        StringCbPrintfW(buf,sizeof(buf), L"%ld", rect.left);
        SetWindowTextW(hwndSta1, buf);

        StringCbPrintfW(buf, sizeof(buf), L"%ld", rect.top);
        SetWindowTextW(hwndSta2, buf);

        break;

    case WM_HOTKEY:
    {
        if ((wParam) == ID_HOTKEY) {
            

                int ret = MessageBoxW(hWnd, L"Are you sure to quit?",
                    L"Message", MB_OKCANCEL);

                if (ret == IDOK) {
                    UnregisterHotKey(hWnd, ID_HOTKEY);
                    SendMessage(hWnd, WM_CLOSE, 0, 0);
                }       
            break;
        }
        if ((wParam) == ID_HOTKEY2) {

            CenterWindow(hWnd);
            break;

        }

    }
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            HPEN hPen = CreatePen(PS_SOLID, 4, RGB(255, 128, 0));

            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void CreateLabels(HWND hwnd) {

    CreateWindowW(L"static", L"x: ",
        WS_CHILD | WS_VISIBLE,
        10, 10, 25, 25,
        hwnd, (HMENU)1, NULL, NULL);

    hwndSta1 = CreateWindowW(L"static", L"150",
        WS_CHILD | WS_VISIBLE,
        40, 10, 55, 25,
        hwnd, (HMENU)2, NULL, NULL);

    CreateWindowW(L"static", L"y: ",
        WS_CHILD | WS_VISIBLE,
        10, 30, 25, 25,
        hwnd, (HMENU)3, NULL, NULL);

    hwndSta2 = CreateWindowW(L"static", L"150",
        WS_CHILD | WS_VISIBLE,
        40, 30, 55, 25,
        hwnd, (HMENU)4, NULL, NULL);
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


void CenterWindow(HWND hwnd) {

    RECT rc = { 0 };

    GetWindowRect(hwnd, &rc);
    int win_w = rc.right - rc.left;
    int win_h = rc.bottom - rc.top;

    int screen_w = GetSystemMetrics(SM_CXSCREEN);
    int screen_h = GetSystemMetrics(SM_CYSCREEN);

    SetWindowPos(hwnd, HWND_TOP, (screen_w - win_w) / 2,
        (screen_h - win_h) / 2, 0, 0, SWP_NOSIZE);
}
void AddMenus(HWND hwnd) {

    HMENU hMenubar;
    HMENU hMenu;

    hMenubar = CreateMenu();
    hMenu = CreateMenu();

    AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&New");
    AppendMenuW(hMenu, MF_STRING, IDM_FILE_OPEN, L"&Open");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&File");
    SetMenu(hwnd, hMenubar);
}

//void Snake(HWND hwnd) {
//    HDC hdc;
//    PAINTSTRUCT ps;
//    hdc = BeginPaint(hwnd, &ps);
//
//    MoveToEx(hdc, 50, 50, NULL);
//    LineTo(hdc, 250, 50);
//
//    HPEN hWhitePen = GetStockObject(WHITE_PEN);
//    HPEN hOldPen = SelectObject(hdc, hWhitePen);
//
//    MoveToEx(hdc, 50, 100, NULL);
//    LineTo(hdc, 250, 100);
//
//    SelectObject(hdc, hOldPen);
//
//    EndPaint(hwnd, &ps);
//}
void PlayerControl() {
    static int playerSpeed = 4;

}