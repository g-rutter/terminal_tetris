#pragma once
#include <array>
#include <math.h>
#include "grid.hpp"

namespace Shapes {
    static const bool x = true;
    static const bool o = false;

    struct Shape {
        static std::array<const Grid, 4> get_rotations(const Grid form) {
            Grid form90 = get_rotation(form);
            Grid form180 = get_rotation(form90);
            return std::array<const Grid, 4>{ form, form90, form180, get_rotation(form180) };
        }

        Shape(const Grid form)
            : grids{get_rotations(form)},
              grid{&grids[0]},
              size{form.n_squares},
              width{grid->grid_size.x}
              {}

        void rotate() {
            current_rotation ++;
            current_rotation %= 4;
            grid = &grids[current_rotation];
            width = grid->grid_size.x;
        }

        static const Grid get_rotation(const Grid current){
            const GridCoord centre{current.grid_size.x, current.grid_size.y};
            const GridCoord inverted_centre{current.grid_size.y, current.grid_size.x};
            Grid rotated{{current.grid_size.y, current.grid_size.x}, current.occupied};
            GridCoord start_coord, end_coord, transformed;
            int end_i;

            for (size_t start_i=0; start_i < current.n_squares; start_i++){
                start_coord = current.to_2D(start_i);
                transformed = (start_coord * 2) - centre + 1;
                end_coord = (GridCoord{-transformed.y, transformed.x} + inverted_centre - 1) / 2;

                end_i = rotated.to_1D(end_coord);
                rotated.occupied.at(end_i) = current.occupied.at(start_i);
            }
            return rotated;
        }
        
        const std::array<const Grid, 4> grids;
        const Grid* grid; // TODO Raw pointer!
        const int size;
        int width;
        int current_rotation = 0;
    };

    const Shape L(
        Grid(GridSize(3), 
             {o, x, o,
              o, x, o,
              o, x, x})
    );

    const Shape T(
        Grid(GridSize(3),
             {o, x, o,
              x, x, x,
              o, o, o})
    );

    const Shape BackwardsL(
        Grid(GridSize(3),
             {o, x, o,
              o, x, o,
              x, x, o})
    );

    const Shape Box(
        Grid(GridSize(2),
             {x, x,
              x, x})
    );

    const Shape I(
        Grid(GridSize(1, 4), {x, x, x, x})
    );

    const std::vector all_shapes = {L, T, BackwardsL, Box, I};
    
    int random_shape() {
        return rand() % all_shapes.size();
    }
}