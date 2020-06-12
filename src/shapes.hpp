#pragma once
#include <array>
#include <math.h>
#include "grid.hpp"

namespace shapes {
    static const bool x = true;
    static const bool o = false;

    class Shape {
        static std::array<const Grid, 4> get_rotations(Grid&& form) {
            const Grid form90 = get_rotation(form);
            const Grid form180 = get_rotation(form90);
            return std::array<const Grid, 4>{ std::move(form), form90, form180, get_rotation(form180) };
        }

        static const Grid get_rotation(const Grid& current){
            const GridCoord centre{current.m_grid_size.x, current.m_grid_size.y};
            const GridCoord inverted_centre{current.m_grid_size.y, current.m_grid_size.x};
            Grid rotated{{current.m_grid_size.y, current.m_grid_size.x}, current.m_occupied};
            GridCoord start_coord, end_coord, transformed;
            int end_i;

            for (size_t start_i=0; start_i < current.m_n_squares; start_i++){
                start_coord = current.to_2D(start_i);
                transformed = (start_coord * 2) - centre + 1;
                end_coord = (GridCoord{-transformed.y, transformed.x} + inverted_centre - 1) / 2;

                end_i = rotated.to_1D(end_coord);
                rotated.m_occupied.at(end_i) = current.m_occupied.at(start_i);
            }
            return rotated;
        }
        
        public:
            Shape(Grid&& form)
                : m_grids{get_rotations(std::move(form))},
                  m_size{form.m_n_squares}
                {}

            const std::array<const Grid, 4> m_grids;
            const size_t m_size;
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

    const std::array all_shapes = {L, T, BackwardsL, Box, I};
    
    int random_shape() {
        return rand() % all_shapes.size();
    }
}