#pragma once
#include "EnumIcon.h"

#include <iostream>

class GridState
{
  public:
    GridState(std::size_t width, std::size_t height);
    ~GridState();

    GridState(const GridState &other) : m_width(other.m_width), m_height(other.m_height), m_grid(other.m_grid)
    {
    }

    GridState(GridState &&other) noexcept : m_width(other.m_width), m_height(other.m_height), m_grid(other.m_grid)
    {
    }

    GridState &operator=(const GridState &other)
    {
        if (this == &other)
            return *this;
        m_width = other.m_width;
        m_height = other.m_height;
        m_grid = other.m_grid;
        return *this;
    }

    GridState &operator=(GridState &&other) noexcept
    {
        if (this == &other)
            return *this;
        m_width = other.m_width;
        m_height = other.m_height;
        m_grid = other.m_grid;
        return *this;
    }

    void SetState(std::size_t x, std::size_t y, CellState state);
    [[nodiscard]] CellState GetState(std::size_t x, std::size_t y) const;

    void Print() const;

    [[nodiscard]] std::size_t GetM_Width() const
    {
        return m_width;
    }

    void SetM_Width(std::size_t m_width)
    {
        this->m_width = m_width;
    }

    [[nodiscard]] std::size_t GetM_Height() const
    {
        return m_height;
    }

    void SetM_Height(std::size_t m_height)
    {
        this->m_height = m_height;
    }

    [[nodiscard]] CellState **GetM_Grid() const
    {
        return m_grid;
    }

    void SetM_Grid(CellState **m_grid)
    {
        this->m_grid = m_grid;
    }

  private:
    std::size_t m_width;
    std::size_t m_height;
    CellState **m_grid;
};
