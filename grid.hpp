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

    const GridSize grid_size;
    const int n_squares;
    std::vector<bool> sediment = std::vector<bool>(n_squares, false);
};