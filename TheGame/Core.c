#include "functions.h"


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
int timer = 180;

TPoint cursor;
LPPOINT cursorPoint;

TObject player;
int playerSpeed = 15;
BOOL newGame = FALSE;
HBITMAP hbtm;
HWND hWnd;

PObject snakeBody;
int result = 0;
TObject apple;
int ticks = 0;




TPoint point(float x, float y) {
    TPoint pt;
    pt.x = x;
    pt.y = y;
    return pt;
}

TDirection direction(BOOL up, BOOL down, BOOL left, BOOL right) {
    TDirection dt;
    dt.up = up;
    dt.down = down;
    dt.left = left;
    dt.right = right;
    return dt;
}

void ObjectInit(TObject* obj, float xPos, float yPos, float width, float height) {
    obj->pos = point(xPos, yPos);
    obj->size = point(width, height);
    obj->brush = RGB(200, 0, 255);
    obj->direction = direction(0, 0, 0, 1);
}

void ObjectShow(TObject head, PObject obj, TObject apple, HDC dc)
{
    SelectObject(dc, GetStockObject(DC_PEN));
    SetDCPenColor(dc, RGB(0, 0, 0));
    SelectObject(dc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(dc, obj->brush);
    Ellipse(dc, (int)(head.pos.x), (int)(head.pos.y),
        (int)(head.pos.x + head.size.x), (int)(head.pos.y + head.size.y));
    for (int i = 0; i < length; i++) {
        Ellipse(dc, (int)(obj[i].pos.x), (int)(obj[i].pos.y),
            (int)(obj[i].pos.x + obj[i].size.x), (int)(obj[i].pos.y + obj[i].size.y));

    }
    SetDCBrushColor(dc, apple.brush);
    Ellipse(dc, (int)(apple.pos.x), (int)(apple.pos.y),
        (int)(apple.pos.x + apple.size.x), (int)(apple.pos.y + apple.size.y));
}

BOOL collision(TObject obj1, TObject obj2) {
    return abs(obj1.pos.x - obj2.pos.x) < ((obj1.size.x + obj2.size.x) / 2 - 5) &&
        abs(obj1.pos.y - obj2.pos.y) < ((obj1.size.y + obj2.size.y) / 2 - 5);
}

void Move(PObject head, PObject body)
{
    TPoint tempy = point(head->pos.x, head->pos.y);

    if (head->direction.up)
    {



        for (int i = 0; i < length - 1; i++) {
            body[i].pos.x = body[i + 1].pos.x;
            body[i].pos.y = body[i + 1].pos.y;
        }
        head->pos.y -= playerSpeed;
        body[length - 1].pos.x = tempy.x;
        body[length - 1].pos.y = tempy.y;
       
    }
    if (head->direction.down)
    {


        for (int i = 0; i < length - 1; i++) {
            body[i].pos.x = body[i + 1].pos.x;
            body[i].pos.y = body[i + 1].pos.y;
        }
        head->pos.y += playerSpeed;
        body[length - 1].pos.x = tempy.x;
        body[length - 1].pos.y = tempy.y;
        
    }
    if (head->direction.left)
    {


        for (int i = 0; i < length - 1; i++) {
            body[i].pos.x = body[i + 1].pos.x;
            body[i].pos.y = body[i + 1].pos.y;
        }
        head->pos.x -= playerSpeed;
        body[length - 1].pos.x = tempy.x;
        body[length - 1].pos.y = tempy.y;
        
    }
    if (head->direction.right)
    {

        for (int i = 0; i < length - 1; i++) {
            body[i].pos.x = body[i + 1].pos.x;
            body[i].pos.y = body[i + 1].pos.y;
        }
        head->pos.x += playerSpeed;
        body[length - 1].pos.x = tempy.x;
        body[length - 1].pos.y = tempy.y;
        
    }

    // TODO: Refactor Move function
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
    if (GetAsyncKeyState('W') < 0 && !player.direction.down && !player.direction.up)
    {
        player.direction = direction(1, 0, 0, 0);
        PlaySound(L"..\\move.wav", NULL, SND_FILENAME | SND_ASYNC);
    };
    if (GetAsyncKeyState('A') < 0 && !player.direction.left && !player.direction.right)
    {
        player.direction = direction(0, 0, 1, 0);
        PlaySound(L"..\\move.wav", NULL, SND_FILENAME | SND_ASYNC);
    }
    if (GetAsyncKeyState('S') < 0 && !player.direction.down && !player.direction.up)
    {
        player.direction = direction(0, 1, 0, 0);
        PlaySound(L"..\\move.wav", NULL, SND_FILENAME | SND_ASYNC);
    }
    if (GetAsyncKeyState('D') < 0 && !player.direction.left && !player.direction.right)
    {
        player.direction = direction(0, 0, 0, 1);
        PlaySound(L"..\\move.wav", NULL, SND_FILENAME | SND_ASYNC);
    }

};

void setApple() {
    apple.pos = point(rand() % 1230 + 16, rand() % 632 + 16);
    apple.size = point(12, 12);
    apple.brush = RGB(rand() % 256, rand() % 256, rand() % 256);
    apple.direction = direction(0, 0, 0, 0);
}

void WinInitial()
{

    timer = 180;
    length = 3;
    result = 0;
    free(snakeBody);
    snakeBody = calloc(length, sizeof(*snakeBody));
    ObjectInit(&player, 500, 500, 15, 15);
    for (int i = 0; i < length; i++) {
        ObjectInit(&snakeBody[i], 500 - (length - i) * 15, 500, 15, 15);
    }


    setApple();
    SetTimer(hWnd, IDT_BASE_TIMER, timer, NULL);

}

void CharMove(HDC hdc) {
    Controls(hdc);
    Move(&player, snakeBody);

}

void Draw(HDC dc) {
    HDC memDC = CreateCompatibleDC(dc);
    HBITMAP memBM = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);
    SelectObject(memDC, memBM);
    SelectObject(memDC, GetStockObject(DC_BRUSH));
    SetDCBrushColor(memDC, RGB(93, 205, 138));
    Rectangle(memDC, 0, 0, 1270, 670);
    Boarders(memDC);
    ObjectShow(player, snakeBody, apple, memDC);
    score(memDC);

    BitBlt(dc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, memDC, 0, 0, SRCCOPY);
    DeleteDC(memDC);
    DeleteObject(memBM);

}

void eatingApple()
{
    if (collision(player, apple))
    {
        setApple();
        timer -= 10;

        SetTimer(hWnd, IDT_BASE_TIMER, timer, NULL);
        result++;
        length++;
        snakeBody = realloc(snakeBody, sizeof(*snakeBody) * length);
        PlaySound(L"..\\collectApple.wav", NULL, SND_FILENAME | SND_ASYNC);
        ObjectInit(&snakeBody[length - 1], snakeBody[length - 2].pos.x, snakeBody[length - 2].pos.y, 15, 15);
    }
}

void update(HDC hdc) {

    if (result == 3)
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
    WinInitial();
    PlaySound(L"..\\champions.wav", NULL, SND_FILENAME | SND_ASYNC);
    newGame = TRUE;
    SetTimer(hWnd, IDT_TIMER2, 3000, NULL);
}

void gameOver(HDC hdc) {

    wchar_t diescreen[] = L"..\\dieimg.bmp";
    LoadImageBtm(hdc, diescreen);
    WinInitial();
    PlaySound(L"..\\dieSound.wav", NULL, SND_FILENAME | SND_ASYNC);
    newGame = TRUE;
    SetTimer(hWnd, IDT_TIMER1, 3000, NULL);
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

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 32, 32, 0);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = backgroundColor;
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_THEGAME);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = (HICON)LoadImage(hInstance, MAKEINTRESOURCE(IDI_ICON2), IMAGE_ICON, 16, 16, 0);;

    return RegisterClassExW(&wcex);
}

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

    hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW & (~WS_MAXIMIZEBOX),
        0, 0, 1290, 755, NULL, NULL, hInstance, NULL);

    if (!hWnd)
    {
        return FALSE;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    return TRUE;
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

