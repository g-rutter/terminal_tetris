#pragma once
#include <vector>
#include <math.h>

struct GridSize{
    explicit constexpr GridSize(const size_t square_width) : x{square_width}, y{square_width} {}
    constexpr GridSize(const size_t x, const size_t y) : x{x}, y{y} {}
    const size_t x;
    const size_t y;
};

struct GridCoord{
    GridCoord operator+(const GridCoord other) const {
        return GridCoord{x + other.x, y + other.y};
    }
    GridCoord operator+(const size_t value) const {
        return GridCoord{x + value, y + value};
    }
    GridCoord operator-(const GridCoord other) const {
        return GridCoord{x - other.x, y - other.y};
    }
    GridCoord operator-(const size_t value) const {
        return GridCoord{x - value, y - value};
    }
    GridCoord operator/(const size_t factor) const {
        return GridCoord{x / factor, y / factor};
    }
    GridCoord operator*(const size_t factor) const {
        return GridCoord{factor * x, factor * y};
    }

    size_t x;
    size_t y;
};

struct Grid{
    Grid(const GridSize grid_size) : m_grid_size{grid_size},
                                     m_n_squares{grid_size.y * grid_size.x} {};

    Grid(const GridSize grid_size, const std::vector<bool> occupied) : Grid(grid_size) {
        m_occupied = occupied;
    }

    GridCoord to_2D(const int i) const {
        return GridCoord{i % m_grid_size.x, i / m_grid_size.x};
    }

    size_t to_1D(const GridCoord coords) const {
        return (coords.y * m_grid_size.x) + coords.x;
    }

    std::vector<size_t> true_indices() const {
        std::vector<size_t> indices;
        for (size_t i=0; i < m_n_squares; i++){
            if (m_occupied.at(i)) indices.push_back(i);
        }
        return indices;
    }

    void absorb(Grid& piece_grid) {
        std::transform(piece_grid.m_occupied.begin(), piece_grid.m_occupied.end(),
                       m_occupied.begin(), m_occupied.begin(), std::logical_or<bool>());
    }

    void zero() {
        std::fill(m_occupied.begin(), m_occupied.end(), false);
    }

    const GridSize m_grid_size;
    const size_t m_n_squares;
    std::vector<bool> m_occupied = std::vector<bool>(m_n_squares, false);
};