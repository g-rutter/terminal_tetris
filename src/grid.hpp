#pragma once
#include <vector>
#include <math.h>

struct GridSize{
    explicit constexpr GridSize(const int square_width) : x{square_width}, y{square_width} {}
    constexpr GridSize(const int x, const int y) : x{x}, y{y} {}
    const int x;
    const int y;
};

struct GridCoord{
    GridCoord operator+(const GridCoord other) const {
        return GridCoord{this->x + other.x, this->y + other.y};
    }
    GridCoord operator+(const int value) const {
        return GridCoord{this->x + value, this->y + value};
    }
    GridCoord operator-(const GridCoord other) const {
        return GridCoord{this->x - other.x, this->y - other.y};
    }
    GridCoord operator-(const int value) const {
        return GridCoord{this->x - value, this->y - value};
    }
    GridCoord operator/(const int factor) const {
        return GridCoord{this->x / factor, this->y / factor};
    }
    GridCoord operator*(const int factor) const {
        return GridCoord{factor * this->x, factor * this->y};
    }

    int x;
    int y;
};

struct Grid{
    Grid(const GridSize grid_size) : grid_size{grid_size},
                                     n_squares{grid_size.y * grid_size.x} {};

    Grid(const GridSize grid_size, const std::vector<bool> overwrite_occupied) : Grid(grid_size) {
        occupied = overwrite_occupied;
    }

    GridCoord to_2D(const int i) const {
        return GridCoord{i % grid_size.x, i / grid_size.x};
    }

    size_t to_1D(const GridCoord coords) const {
        return (coords.y * grid_size.x) + coords.x;
    }

    std::vector<size_t> true_indices() const {
        std::vector<size_t> indices;
        for (size_t i=0; i < n_squares; i++){
            if (occupied.at(i)) indices.push_back(i);
        }
        return indices;
    }

    void absorb(Grid piece_grid) {
        for (auto &&i : piece_grid.true_indices()) occupied[i] = true;
    }

    void zero() {
        occupied = std::vector<bool>(n_squares, false);
    }

    const GridSize grid_size;
    const int n_squares;
    std::vector<bool> occupied = std::vector<bool>(n_squares, false);
};