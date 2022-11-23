#pragma once

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
#include "Structs.h"

void CenterWindow(HWND);
HWND hwndSta1;
HWND hwndSta2;
void CreateLabels(HWND);
void AddMenus(HWND);
HWND hWnd;
HDC hdc;
HBITMAP scoreNum;
BOOL enterFlag;
int length;
int timer;


TPoint cursor;
LPPOINT cursorPoint;

TObject player;

extern int playerSpeed;
BOOL newGame;
HBITMAP hbtm;
HWND hWnd;

PObject snakeBody;
int result;
TObject apple;
int ticks;

RECT rect;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


extern wchar_t numbers[10][9];

TPoint point(float x, float y);
TDirection direction(BOOL up, BOOL down, BOOL left, BOOL right);

void ObjectInit(TObject* obj, float xPos, float yPos, float width, float height);
void ObjectShow(TObject head, PObject obj, TObject apple, HDC dc);
BOOL collision(TObject obj1, TObject obj2);

void Move(PObject head, PObject body);
void Controls(HDC hdc);
void setApple();
void WinInitial();
void CharMove(HDC);
void Draw(HDC dc);
void eatingApple();
void update(HDC hdc);

void LoadImageBtm(HDC hdc, wchar_t path[]);
void score(HDC hdc);
void winGame(HDC hdc);
void gameOver(HDC hdc);
void Boarders(HDC hdc);

void CenterWindow(HWND hwnd);
void AddMenus(HWND hwnd);