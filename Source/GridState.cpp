#include "GridState.h"
#include <iostream>

GridState::GridState(std::size_t width, std::size_t height)
{
    m_width = width;
    m_height = height;
    m_grid = new CellState *[height];
    for (std::size_t y = 0; y < height; ++y)
        m_grid[y] = new CellState[width];
}

GridState::~GridState()
{
    for (std::size_t y = 0; y < m_height; ++y)
        delete[] m_grid[y];
    delete[] m_grid;
}

void GridState::SetState(std::size_t x, std::size_t y, CellState state)
{
    if (x >= m_width || y >= m_height)
        return;

    if (x < m_width && y < m_height)
        m_grid[y][x] = state;
}

CellState GridState::GetState(std::size_t x, std::size_t y) const
{
    if (x < m_width && y < m_height)
        return m_grid[y][x];
    return CellState::dead;
}

void GridState::Print() const
{
    for (std::size_t y = 0; y < m_height; ++y)
    {
        for (std::size_t x = 0; x < m_width; ++x)
        {
            std::wcout << ((m_grid[y][x] == CellState::alive) ? L"▓▓" : L"  ");
        }
        std::wcout << L"\n";
    }
}
