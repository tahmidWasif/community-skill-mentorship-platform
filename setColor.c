#include "setColor.h"

#include <stdio.h>
#include <windows.h>

void set_color(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}