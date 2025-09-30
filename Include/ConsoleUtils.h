#pragma once
#include <iostream>
#include <string>
#include <windows.h>

void ClearScreen();

void PrintAtPosition(int x, int y, const std::wstring &text);

std::wstring CenterText(const std::wstring &text, int width);

void DrawBorder();
void HideCursor();
void DrawBottomBorder();

extern const int ConsoleWidth;
extern const int ConsoleHeight;
