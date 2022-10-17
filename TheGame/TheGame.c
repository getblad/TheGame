// TheGame.cpp : Defines the entry point for the application.
//
#pragma comment(linker, "\"/manifestdependency:type='Win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#pragma comment (lib, "winmm.lib")

#include "framework.h"
#include "TheGame.h"
#include <windows.h>
#include  "strsafe.h"
#include <mmsystem.h>

#include <time.h>
#include <stdlib.h>
#include <math.h>


#define MAX_LOADSTRING 100
#define ID_HOTKEY 1
#define ID_HOTKEY2 2
#define ID_HOTKEY_CONTINUE 3

#define IDM_FILE_NEW 1
#define IDM_FILE_QUIT 2

#define IDT_TIMER1 1
#define IDT_BASE_TIMER 2
#define IDT_TIMER2 3



wchar_t numbers[10][9] = { L"..\\0.bmp", L"..\\1.bmp" ,L"..\\2.bmp" , L"..\\3.bmp", L"..\\4.bmp",
L"..\\5.bmp",L"..\\6.bmp", L"..\\7.bmp", L"..\\8.bmp", L"..\\9.bmp" };
  
void CenterWindow(HWND);
HWND hwndSta1;
HWND hwndSta2;
void CreateLabels(HWND);
void AddMenus(HWND);
HWND hWnd;
HDC hdc;
HBITMAP scoreNum;
BOOL enterFlag = FALSE;

int length = 3;
int timer = 200;

typedef struct SPoint {
    float x, y;
} TPoint ;

typedef struct SDirection {
    BOOL up, down, left, right;
} TDirection;

TPoint point(float x, float y) {
    TPoint pt;
    pt.x = x;
    pt.y = y;
    return pt;
}

TDirection direction(BOOL up, BOOL down, BOOL left, BOOL right ) {
    TDirection dt;
    dt.up = up;
    dt.down = down;
    dt.left = left;
    dt.right = right;
    return dt;
}

typedef struct SObject {
    TPoint pos;
    TPoint size;
    COLORREF brush;
    TDirection direction;
} TObject, *PObject;

void ObjectInit(TObject* obj, float xPos, float yPos, float width, float height) {
    obj -> pos = point(xPos, yPos);
    obj->size = point(width, height);
    obj->brush = RGB(200, 0, 255);
    obj->direction = direction(0,0,0,1);
}

void ObjectShow(TObject head, PObject obj, TObject apple, HDC dc)
{
    SelectObject(dc, GetStockObject(DC_PEN));
    SetDCPenColor(dc, RGB(0, 0, 0));
    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, obj->brush);
    Ellipse(dc, (int)(head.pos.x), (int)(head.pos.y),
        (int)(head.pos.x + head.size.x), (int)(head.pos.y + head.size.y));
    for (int i = 0; i <= length; i++) {
        Ellipse(dc, (int)(obj[i].pos.x), (int)(obj[i].pos.y),
            (int)(obj[i].pos.x + obj[i].size.x), (int)(obj[i].pos.y + obj[i].size.y));

    }
    SetDCBrushColor(dc, apple.brush);
    Ellipse(dc, (int)(apple.pos.x), (int)(apple.pos.y),
        (int)(apple.pos.x + apple.size.x), (int)(apple.pos.y + apple.size.y));
}

TPoint cursor;
LPPOINT cursorPoint;
void  winGame(HDC);
TObject player;
void score(HDC);
int playerSpeed = 15;
BOOL newGame = FALSE;
HBITMAP hbtm;
HWND hWnd;
void gameOver(HDC);
PObject snakeBody = NULL;
int result = 0;
TObject apple;
BOOL collision(TObject obj1, TObject obj2) {
    if (abs(obj1.pos.x  - obj2.pos.x) < ((obj1.size.x + obj2.size.x)/2) &&
        abs(obj1.pos.y - obj2.pos.y) < ((obj1.size.y + obj2.size.y) / 2))
    {
        return TRUE;
    }
    return FALSE;
}

void Move(PObject head, PObject body)
{
    TPoint tempy = point(head->pos.x, head->pos.y);
    
    if (head -> direction.up)
    {
       
       
         
        for (int i = 0; i < length-1; i++) {
            body[i].pos.x = body[i + 1].pos.x;
            body[i].pos.y = body[i + 1].pos.y;
        }
        head->pos.y -= playerSpeed;
        body[length-1].pos.x = tempy.x;
        body[length-1].pos.y = tempy.y;
        if (player.size.x > player.size.y)
        {
            player.size.x = player.size.x + player.size.y;
            player.size.y = player.size.x - player.size.y;
            player.size.x = player.size.x - player.size.y;
        }
    }
    if (head->direction.down)
    {
      
        
        for (int i = 0; i < length - 1; i++) {
            body[i].pos.x = body[i + 1].pos.x;
            body[i].pos.y = body[i + 1].pos.y;
        }
        head->pos.y += playerSpeed;
        body[length-1].pos.x = tempy.x;
        body[length-1].pos.y = tempy.y;
        if (player.size.x > player.size.y)
        {
            player.size.x = player.size.x + player.size.y;
            player.size.y = player.size.x - player.size.y;
            player.size.x = player.size.x - player.size.y;
        };
    }
    if (head->direction.left)
    {
       
       
        for (int i = 0; i < length - 1; i++) {
            body[i].pos.x = body[i + 1].pos.x;
            body[i].pos.y = body[i + 1].pos.y;
        }
        head->pos.x -= playerSpeed;
        body[length-1].pos.x = tempy.x;
        body[length-1].pos.y = tempy.y;
        if (player.size.x < player.size.y)
        {
            player.size.x = player.size.x + player.size.y;
            player.size.y = player.size.x - player.size.y;
            player.size.x = player.size.x - player.size.y;
        }
    }
    if (head->direction.right)
    {
       
        for (int i = 0; i < length - 1; i++) {
            body[i].pos.x = body[i + 1].pos.x;
            body[i].pos.y = body[i + 1].pos.y;
        }
        head->pos.x += playerSpeed;
        body[length-1].pos.x = tempy.x;
        body[length-1].pos.y = tempy.y;
        if (player.size.x < player.size.y)
        {
            player.size.x = player.size.x + player.size.y;
            player.size.y = player.size.x - player.size.y;
            player.size.x = player.size.x - player.size.y;
        }
    }
}

void Controls(HDC hdc)
{
    if (player.pos.y < 9 || player.pos.y > 639) {
        player.direction = direction(0, 0, 0, 0);
        Sleep(500);
        gameOver(hdc);
        

    }
    if (player.pos.x < 9 || player.pos.x > 1237) {
        player.direction = direction(0, 0, 0, 0);
        Sleep(500);
        gameOver(hdc);
        
    }
    if (GetKeyState('W') < 0 && (player.direction.down != 1)) player.direction = direction(1, 0, 0, 0);
    if (GetKeyState('A') < 0 && (player.direction.right != 1)) player.direction = direction(0, 0, 1, 0);
    if (GetKeyState('S') < 0 && (player.direction.up != 1)) player.direction = direction(0, 1, 0, 0);
    if (GetKeyState('D') < 0 && (player.direction.left != 1)) player.direction = direction(0, 0, 0, 1);

};

RECT rect;
void setApple() {
    apple.pos = point(rand() % 1230 + 16, rand() % 632 + 16);
    apple.size = point(12, 12);
    apple.brush = RGB(rand() % 256, rand() % 256, rand() % 256);
    apple.direction = direction(0, 0, 0, 0);
}



void WinInitial()
{
    
    timer = 200;
    length = 3;
    result = 0;
    ObjectInit(&player, 500, 500, 15, 15);
    for (int i = length; i >= 0; i--) {
        ObjectInit(&snakeBody[length-i-1], 500 - i * 15-15  , 500, 15, 15);
        

    }

   
    setApple();
    SetTimer(hWnd, IDT_BASE_TIMER, timer, NULL);
        
}


void CharMove(hdc) {
    Controls(hdc);
    Move(&player, snakeBody);

}

void Boarders(HDC hdc);

PObject CreateObject()
{
    length++;
    PObject temp = realloc(snakeBody, sizeof(*snakeBody) * length);
    if (temp == NULL) {       
        free(snakeBody);
        exit(EXIT_FAILURE);
    }
    else {
        snakeBody = temp;
    }
    return snakeBody + length - 1;
}


void Draw(HDC dc) {
    HDC memDC = CreateCompatibleDC(dc);
    HBITMAP memBM = CreateCompatibleBitmap(dc,rect.right- rect.left, rect.bottom - rect.top );
    SelectObject(memDC, memBM);
    SelectObject(memDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(memDC, RGB(255, 255, 255));
    Rectangle(memDC, 0, 0, 1280, 720);
    Boarders(memDC);
    ObjectShow(player, snakeBody, apple, memDC);
    score(memDC);

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




void eatingApple()
{
    if (collision(player, apple))
    {
        setApple();
        timer -= 20;
        
        SetTimer(hWnd, IDT_BASE_TIMER, timer, NULL);
        result++;
        length++;
        //snakeBody = realloc(snakeBody, sizeof(*snakeBody) * length);
        ObjectInit(&snakeBody[length-1], snakeBody[length - 2].pos.x, snakeBody[length - 2].pos.y, 15, 15);
   }
}

void update(HDC hdc) {
    
    if (result == 10)
    {
        winGame(hdc);
    }
    CharMove(hdc);
    for (int i = 0; i < length; i++) {
        if (collision(player, snakeBody[i]))
        {
            gameOver(hdc);
        }
    }
    
    if (!newGame) {
        eatingApple();
        Draw(hdc);
    }

}
void LoadImageBtm(HDC hdc, wchar_t path[]) {

    PAINTSTRUCT ps;
    BITMAP bitmap;
    HDC hdcMem;
    HGDIOBJ oldBitmap;

    hbtm = LoadImageW(NULL, path,
        IMAGE_BITMAP, 1280, 720, LR_LOADFROMFILE);
   
    hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, hbtm);
    GetObject(hbtm, sizeof(bitmap), &bitmap);
    BitBlt(hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight,
        hdcMem, 0, 0, SRCCOPY);
    //SelectObject(hdcMem, oldBitmap);
    DeleteDC(hdcMem);
    DeleteObject(hbtm);
    
}

void score(HDC hdc) {
    HDC hdcMem;
    BITMAP bitmap;

    scoreNum = LoadImageW(NULL, numbers[result], IMAGE_BITMAP, 50, 75, LR_LOADFROMFILE);
    hdcMem = CreateCompatibleDC(hdc);
    SelectObject(hdcMem, scoreNum);
    GetObject(scoreNum, sizeof(bitmap), &bitmap);
    BitBlt(hdc, 1200, 30, bitmap.bmWidth, bitmap.bmHeight,
        hdcMem, 0, 0, SRCCOPY);
    DeleteDC(hdcMem);
    DeleteObject(scoreNum);
   
}

void winGame(HDC hdc) {

    wchar_t winscreen[] = L"..\\win.bmp";
    LoadImageBtm(hdc, winscreen);
    //snakeBody = calloc(length, sizeof(*snakeBody));
    WinInitial();
    PlaySound(L"..\\champions.wav", NULL, SND_FILENAME | SND_ASYNC);
    newGame = TRUE;
    
    //snakeBody = realloc(snakeBody, sizeof(*snakeBody) * length);
    SetTimer(hWnd, IDT_TIMER2, 3000, NULL);
}

void gameOver(HDC hdc) {
    
    wchar_t diescreen[] = L"..\\dieimg.bmp";
    LoadImageBtm(hdc, diescreen);
    //snakeBody = calloc(length, sizeof(*snakeBody));
    WinInitial();
    PlaySound(L"..\\dieSound.wav", NULL, SND_FILENAME|SND_ASYNC);
    newGame = TRUE;
    
    //snakeBody = realloc(snakeBody, sizeof(*snakeBody) * length);
    SetTimer(hWnd, IDT_TIMER1, 3000, NULL);
    
    //DeleteDC(hdc);
    //HWND hwndMem = WindowFromDC(hdc);
    //
    //CreateWindow(
    //    L"BUTTON",  // Predefined class; Unicode assumed 
    //    L"Restart",      // Button text 
    //    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
    //    700,         // x position 
    //    500,         // y position 
    //    100,        // Button width
    //    100,        // Button height
    //    hwndMem,     // Parent window
    //    IDM_FILE_NEW,       // No menu.
    //    (HINSTANCE)GetWindowLongPtr(hwndMem, GWLP_HINSTANCE),
    //    NULL);

    //CreateWindow(
    //    L"BUTTON",  // Predefined class; Unicode assumed 
    //    L"Quite",      // Button text 
    //    WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles 
    //    1000,         // x position 
    //    500,         // y position 
    //    100,        // Button width
    //    100,        // Button height
    //    hwndMem,     // Parent window
    //    IDM_FILE_QUIT,       // No menu.
    //    (HINSTANCE)GetWindowLongPtr(hwndMem, GWLP_HINSTANCE),
    //    NULL);
    

}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    srand(time(NULL));
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
    snakeBody = calloc(10, sizeof(*snakeBody));
    srand(time(NULL));
    
    
    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX),
       0, 0, 1290, 755, NULL, NULL, hInstance, NULL);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    hdc = GetDC(hWnd);
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_THEGAME));

    MSG msg;

    WinInitial();
    //update(hdc);
   
    
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

        //GetCursorPos(cursorPoint);

        //ScreenToClient(hWnd, cursorPoint);
        //cursor.x = cursorPoint[0].x;
        //cursor.y = cursorPoint[0].y;
      
       
    }

    return (int) msg.wParam;
}

void Boarders(HDC hdc)
{
    LOGBRUSH brush;
    COLORREF col = RGB(0, 0, 0);
    DWORD pen_style = PS_SOLID | PS_JOIN_MITER | PS_GEOMETRIC;

    brush.lbStyle = BS_SOLID;
    brush.lbColor = col;
    brush.lbHatch = 0;

    

    HPEN hPen1 = ExtCreatePen(pen_style, 8, &brush, 0, NULL);
    HPEN holdPen = SelectObject(hdc, hPen1);

    POINT points[5] = { { 8, 8 }, { 1260, 8 }, { 1260, 660 },
        { 8, 660 }, { 8, 8} };
    Polygon(hdc, points, 5);
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

//void RegisterDeathScreen(void) {
//
//    HBRUSH hbrush = CreateSolidBrush(RGB(90, 90, 90));
//
//    WNDCLASSW rwc = { 0 };
//
//    rwc.lpszClassName = L"RedPanelClass";
//    rwc.hbrBackground = hbrush;
//    rwc.lpfnWndProc = DeathScreenProc;
//    rwc.hCursor = LoadCursor(0, IDC_ARROW);
//    RegisterClassW(&rwc);
//}
//
//LRESULT CALLBACK DeathScreenProc(HWND hwnd, UINT msg,
//    WPARAM wParam, LPARAM lParam)
//{
//    switch (msg) {
//
//    case WM_LBUTTONUP:
//        newGame = TRUE;
//        break;
//
//        return DefWindowProcW(hwnd, msg, wParam, lParam);
//    };
//}

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
    //wchar_t buf[10];
    
    switch (message)
    {
    case WM_CREATE:

        RegisterHotKey(hWnd, ID_HOTKEY, MOD_CONTROL, 0x43);
        RegisterHotKey(hWnd, ID_HOTKEY2, NULL, 0x52);
        //CreateLabels(hWnd);
        GetWindowRect(hWnd, &rect);
        AddMenus(hWnd);
        SetTimer(hWnd, IDT_BASE_TIMER, 200, NULL);
        break;
    case WM_KEYDOWN:

        if (wParam == VK_ESCAPE) {

            int ret = MessageBoxW(hWnd, L"Are you sure to quit?",
                L"Message", MB_OKCANCEL);

            if (ret == IDOK) {

                SendMessage(hWnd, WM_CLOSE, 0, 0);
            }
        }
        if (wParam == VK_RETURN && enterFlag == TRUE) {

            newGame = FALSE;
            enterFlag = FALSE;
            PlaySound(0, 0, 0);
        }
        break;
        break;
    case WM_COMMAND:

        switch (LOWORD(wParam)) {
        case IDM_FILE_NEW:

            newGame = TRUE;
            break;

        case IDM_FILE_QUIT:

            SendMessage(hWnd, WM_CLOSE, 0, 0);
            break;
       /* case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);*/
        }

        break;
    case WM_TIMER:
        switch (wParam)
        {
        case IDT_TIMER1:
            // process the gameplay timer
        {

        LoadImageBtm(hdc, L"..\\dieimg2.bmp");
        KillTimer(hWnd, IDT_TIMER1);
        enterFlag = TRUE;
        
        return 0;
        }

        case IDT_TIMER2:
            // process the gameplay timer
        {

            
            KillTimer(hWnd, IDT_TIMER1);
            enterFlag = TRUE;

            return 0;
        }
        case IDT_BASE_TIMER:
            if (!newGame) {
                update(hdc);
            }
                
        }
    case WM_MOVE:

        /*GetWindowRect(hWnd, &rect);*/

        /*StringCbPrintfW(buf,sizeof(buf), L"%ld", rect.left);
        SetWindowTextW(hwndSta1, buf);

        StringCbPrintfW(buf, sizeof(buf), L"%ld", rect.top);
        SetWindowTextW(hwndSta2, buf);*/

        break;
    case WM_LBUTTONDOWN:


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

            /*gameOver(hdcMem);*/
            //newGame = TRUE;
            //free(snakeBody);
            length = 3;
            WinInitial();
            break;

        }

    }
    case WM_PAINT:
        {
            
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            
            //HPEN hPen = CreatePen(PS_SOLID, 4, RGB(255, 128, 0));
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        SendMessage(hWnd, WM_CLOSE, 0, 0);
        break;
    default:
       
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

//void CreateLabels(HWND hwnd) {
//
//    CreateWindowW(L"static", L"x: ",
//        WS_CHILD | WS_VISIBLE,
//        10, 10, 25, 25,
//        hwnd, (HMENU)1, NULL, NULL);
//
//    hwndSta1 = CreateWindowW(L"static", L"150",
//        WS_CHILD | WS_VISIBLE,
//        40, 10, 55, 25,
//        hwnd, (HMENU)2, NULL, NULL);
//
//    CreateWindowW(L"static", L"y: ",
//        WS_CHILD | WS_VISIBLE,
//        10, 30, 25, 25,
//        hwnd, (HMENU)3, NULL, NULL);
//
//    hwndSta2 = CreateWindowW(L"static", L"150",
//        WS_CHILD | WS_VISIBLE,
//        40, 30, 55, 25,
//        hwnd, (HMENU)4, NULL, NULL);
//}

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

    AppendMenuW(hMenu, MF_STRING, IDM_FILE_NEW, L"&Restart\tR");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit\tEsc");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR)hMenu, L"&Game");
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
