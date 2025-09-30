#include "ConsoleUtils.h"

const int ConsoleWidth = 80;
const int ConsoleHeight = 25;

void ClearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0};
    DWORD count;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    FillConsoleOutputCharacter(hConsole, L' ', csbi.dwSize.X * csbi.dwSize.Y, coord, &count);
    SetConsoleCursorPosition(hConsole, coord);
}

void PrintAtPosition(int x, int y, const std::wstring &text)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = {(SHORT)x, (SHORT)y};
    SetConsoleCursorPosition(hConsole, pos);
    std::wcout << text;
}

std::wstring CenterText(const std::wstring &text, int width)
{
    int pad = (width - static_cast<int>(text.length())) / 2;
    if (pad > 0)
        return std::wstring(pad, L' ') + text;
    return text;
}

void DrawBorder()
{
    for (int x = 0; x < ConsoleWidth; ++x)
        PrintAtPosition(x, 0, L"═");
    for (int y = 1; y < ConsoleHeight - 1; ++y)
    {
        PrintAtPosition(0, y, L"║");
        PrintAtPosition(ConsoleWidth - 1, y, L"║");
    }
    PrintAtPosition(0, 0, L"╔");
    PrintAtPosition(ConsoleWidth - 1, 0, L"╗");
    PrintAtPosition(0, ConsoleHeight - 1, L"╚");
    PrintAtPosition(ConsoleWidth - 1, ConsoleHeight - 1, L"╝");
}

void DrawBottomBorder()
{
    for (int x = 1; x < ConsoleWidth - 1; ++x)
        PrintAtPosition(x, ConsoleHeight - 1, L"═");
}

void HideCursor()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}
