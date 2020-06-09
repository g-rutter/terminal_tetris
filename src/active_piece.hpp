#pragma once

#include "grid.hpp"
#include "shapes.hpp"

struct ActivePiece {
    ActivePiece(const Shapes::Shape shape, const Grid& sediment_grid)
        : shape{shape},
          sediment_grid{sediment_grid},
          global_grid{sediment_grid.grid_size, std::vector<bool>(sediment_grid.n_squares, false)},
          shadow_grid{sediment_grid.grid_size, std::vector<bool>(sediment_grid.n_squares, false)}
    {
        shape_loc.x = (global_grid.grid_size.x - shape.width) / 2;
        update_grids();
    };

    void down() {
        shape_loc.y++;
        if(!update_grids()){
            shape_loc.y--;
            update_grids();
            landed = true;
        }
    }

    void fall() {
        do {
            shape_loc.y++;
        } while (update_grids());
        shape_loc.y--;
        update_grids();
        landed = true;
    }

    void left() {
        shape_loc.x--;
        if(!update_grids()){
            shape_loc.x++;
            update_grids();
        }
    }

    void right() {
        shape_loc.x++;
        if(!update_grids()){
            shape_loc.x--;
            update_grids();
        }
    }

    void rotate() {
        int recentre_amount = (shape.grid->grid_size.x - shape.grid->grid_size.y) / 2;
        do {
            shape.rotate();
            shape_loc.x += recentre_amount;
            shape_loc.y -= recentre_amount;
            recentre_amount = -recentre_amount;
        } while (!update_grids());
    }

    bool update_grids() {
        // Update the global grid giving the current piece's occupancy.
        // bool return code denotes VALIDITY OF STATE.
        // Update does not complete if state is invalid.
        int global_i, sediment_i;
        std::fill(global_grid.occupied.begin(), global_grid.occupied.end(), false);
        std::fill(shadow_grid.occupied.begin(), shadow_grid.occupied.end(), false);
        GridCoord global_xy, sediment_xy;

        for(size_t& local_i : shape.grid->true_indices()){
            global_xy = shape.grid->to_2D(local_i) + shape_loc;
            global_i = global_grid.to_1D(global_xy);
            if (global_i >= global_grid.n_squares) return false; // Fallen off bottom
            else if (global_xy.x < 0 || global_xy.x >= global_grid.grid_size.x) return false ; // Off the sides
            else if (sediment_grid.occupied.at(global_i)) return false; // Piece overlaps with sediment (previous pieces)
            global_grid.occupied.at(global_i) = true;

            // Put shadow straight on top of sediment directly below shape.
            sediment_xy = {global_xy.x, sediment_grid.grid_size.y};
            do {
                sediment_xy.y--;
                sediment_i = sediment_grid.to_1D(sediment_xy);
            } while(sediment_grid.occupied.at(sediment_i));
            shadow_grid.occupied.at(sediment_i) = true;
        }
        return true;
    }

    private:
        Shapes::Shape shape;
        const Grid& sediment_grid;
        GridCoord shape_loc{0, 0};

    public:
        bool landed{false};
        Grid global_grid, shadow_grid;
};