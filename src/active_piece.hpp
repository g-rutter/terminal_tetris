#pragma once

#include "grid.hpp"
#include "shapes.hpp"

struct ActivePiece {
    ActivePiece(const Shapes::Shape shape, const Grid& sediment_grid)
        : shape{shape},
          sediment_grid{sediment_grid},
          global_grid{sediment_grid.grid_size, std::vector<bool>(sediment_grid.n_squares, false)}
    {
        x_loc = (global_grid.grid_size.x - shape.width) / 2;
        update_global_grid();
    };

    void down() {
        y_loc++;
        if(!update_global_grid()){
            y_loc--;
            update_global_grid();
            landed = true;
        }
    }

    void fall() {
        do {
            y_loc++;
        } while (update_global_grid());
        y_loc--;
        update_global_grid();
        landed = true;
    }

    void left() {
        x_loc--;
        if(!update_global_grid()){
            x_loc++;
            update_global_grid();
        }
    }

    void right() {
        x_loc++;
        if(!update_global_grid()){
            x_loc--;
            update_global_grid();
        }
    }

    void rotate() {
        do {
            shape.rotate();
        } while (!update_global_grid());
    }

    bool update_global_grid() {
        // Update the global grid giving the current piece's occupancy.
        // bool return code denotes VALIDITY OF STATE.
        // Update does not complete if state is invalid.
        int global_x, global_y, global_i;
        std::fill(global_grid.occupied.begin(), global_grid.occupied.end(), false);

        for(size_t local_i=0; local_i< shape.size; local_i++) {
            if(shape.data()[local_i]) {
                global_x = x_loc + (local_i % shape.width);
                global_y = y_loc + (local_i / shape.width);
                global_i = (global_y * sediment_grid.grid_size.x) + global_x;
                if (global_i >= global_grid.n_squares) return false; // Fallen off bottom
                else if (global_x < 0 || global_x >= global_grid.grid_size.x) return false ; // Off the sides
                else if (sediment_grid.occupied[global_i]) return false; // Piece overlaps with sediment (previous pieces)
                global_grid.occupied[global_i] = true;
            }
        }
        return true;
    }

    private:
        Shapes::Shape shape;
        const Grid& sediment_grid;
        int x_loc{0};
        int y_loc{0};

    public:
        bool landed{false};
        Grid global_grid;
};