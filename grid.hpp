#pragma once
#include <vector>

struct GridSize{
    int x;
    int y;
};

struct GridCoord{
    int x;
    int y;
};

struct Grid{
    Grid(const GridSize grid_size) : grid_size{grid_size},
                                     n_squares{grid_size.y * grid_size.x} {};

    Grid(const GridSize grid_size, std::vector<bool> overwrite_occupied) : Grid(grid_size) {
        occupied = overwrite_occupied;
    }

    GridCoord to_2D(const int i) const {
        return GridCoord{i % grid_size.x, i / grid_size.x};
    }

    void absorb(Grid piece_grid) {
        for(size_t i=0; i < n_squares; i++){
            if (piece_grid.occupied[i]) occupied[i] = true;
        }
    }

    void zero() {
        occupied = std::vector<bool>(n_squares, false);
    }

    const GridSize grid_size;
    const int n_squares;
    std::vector<bool> occupied = std::vector<bool>(n_squares, false);
};