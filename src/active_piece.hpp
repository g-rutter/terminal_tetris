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
        std::fill(global_grid.occupied.begin(), global_grid.occupied.end(), false);
        bool valid = update_grid(sediment_grid, global_grid, shape, shape_loc);

        if (valid) {
            // shadow grid
            GridCoord shadow_loc = {shape_loc.x, shape_loc.y + 1};
            while(update_grid(sediment_grid, shadow_grid, shape, shadow_loc)) {
                shadow_loc.y++;
            }
            shadow_loc.y--;
            std::fill(shadow_grid.occupied.begin(), shadow_grid.occupied.end(), false);
            if (shadow_loc.y - 3 > shape_loc.y) { // Don't show shadow if it might be visually distracting.
                update_grid(sediment_grid, shadow_grid, shape, shadow_loc);
            }
        } 
        return valid;
    }
    
    bool update_grid(const Grid& sediment_grid, Grid& update_grid, const Shapes::Shape& shape, const GridCoord& shape_loc){
        GridCoord global_xy;
        int global_i;

        for(size_t& local_i : shape.grid->true_indices()){
            global_xy = shape.grid->to_2D(local_i) + shape_loc;
            global_i = update_grid.to_1D(global_xy);
            if (global_i >= update_grid.n_squares) return false; // Fallen off bottom
            else if (global_xy.x < 0 || global_xy.x >= update_grid.grid_size.x) return false ; // Off the sides
            else if (sediment_grid.occupied.at(global_i)) return false; // Piece overlaps with sediment (previous pieces)
            update_grid.occupied.at(global_i) = true;
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