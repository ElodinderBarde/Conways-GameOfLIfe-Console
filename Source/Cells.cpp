#include "Cells.h"

#include "GridState.h"

int CheckNeighbor(const GridState &grid, std::size_t x, std::size_t y)
{
    int counter = 0;
    const int width = static_cast<int>(grid.GetM_Width());
    const int height = static_cast<int>(grid.GetM_Height());

    for (int dy = -1; dy <= 1; ++dy)
    {
        for (int dx = -1; dx <= 1; ++dx)
        {
            if (dx == 0 && dy == 0)
                continue;

            int nx = static_cast<int>(x) + dx;
            int ny = static_cast<int>(y) + dy;

            if (nx >= 0 && nx < width && ny >= 0 && ny < height)
            {
                if (grid.GetState(nx, ny) == CellState::alive)
                {
                    ++counter;
                }
            }
        }
    }

    return counter;
}
