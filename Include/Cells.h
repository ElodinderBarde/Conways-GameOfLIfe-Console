#pragma once
#include "EnumIcon.h"
#include "structPosition.h"
#include <string>

class GridState;
class Cell
{
  private:
    CellState state;
    Position pos;

  public:
    Cell() : state(CellState::dead), pos({0, 0})
    {
    }
    Cell(CellState state, Position pos) : state(state), pos(pos)
    {
    }

    CellState State() const
    {
        return state;
    }
    void SetState(CellState newState)
    {
        state = newState;
    }

    Position Pos() const
    {
        return pos;
    }
    void SetPos(Position newPos)
    {
        pos = newPos;
    }

    [[nodiscard]] std::wstring GetIcon() const
    {
        return (state == CellState::alive) ? L"■" : L"□";
    }
};

int CheckNeighbor(const GridState &grid, std::size_t x, std::size_t y);
