// TheGame.cpp : Defines the entry point for the application.
//

#include "functions.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    
    MSG msg;
    srand(time(NULL));
    MessageBoxW(NULL, L"TheBestGameEVER!!", L"First", MB_OK);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_THEGAME, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    InitInstance(hInstance, nCmdShow);

    hdc = GetDC(hWnd);
    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_THEGAME));

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
      
       
    }

    return (int) msg.wParam;
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
    case WM_CREATE:

        CenterWindow(hWnd);
        RegisterHotKey(hWnd, ID_HOTKEY, MOD_CONTROL, 0x43);
        RegisterHotKey(hWnd, ID_HOTKEY2, NULL, 0x52);
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
       
        }

        break;
    case WM_TIMER:
        switch (wParam)
        {
        case IDT_TIMER1:
            
        {

        LoadImageBtm(hdc, L"..\\dieimg2.bmp");
        KillTimer(hWnd, IDT_TIMER1);
        enterFlag = TRUE;
        
        return 0;
        }

        case IDT_TIMER2:
           
        {            
            KillTimer(hWnd, IDT_TIMER2);
            enterFlag = TRUE;

            return 0;
        }
        case IDT_BASE_TIMER:
            if (!newGame) {
                update(hdc);
                return 0;
            }
                
        }


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

            WinInitial();
            break;

        }

    }
    case WM_PAINT:
        {
            
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
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



