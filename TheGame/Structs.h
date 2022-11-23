#pragma once

#include <windows.h>

#define MAX_LOADSTRING 100
#define ID_HOTKEY 1
#define ID_HOTKEY2 2
#define ID_HOTKEY_CONTINUE 3

#define IDM_FILE_NEW 1
#define IDM_FILE_QUIT 2

#define IDT_TIMER1 1
#define IDT_BASE_TIMER 2
#define IDT_TIMER2 3




typedef struct SPoint {
    float x, y;
} TPoint;

typedef struct SDirection {
    BOOL up, down, left, right;
} TDirection;


typedef struct SObject {
    TPoint pos;
    TPoint size;
    COLORREF brush;
    TDirection direction;
} TObject, * PObject;


