#pragma once

#include "grid.hpp"
#include "shapes.hpp"

struct ActivePiece {
    ActivePiece(const Shapes::Shape shape, const Grid& sediment_grid)
        : shape{shape},
          sediment_grid{sediment_grid},
          global_grid{sediment_grid.grid_size, std::vector<bool>(sediment_grid.n_squares, false)}
    {
        update_global_grid();
    };
    ActivePiece(const Grid& grid) : ActivePiece(Shapes::random_shape(), grid) {};

    void down() {
        y_loc++;
        update_global_grid();
        if(!valid_state()){
            y_loc--;
            update_global_grid();
            landed = true;
        }
    }

    void fall() {
        while(valid_state()){
            y_loc++;
            update_global_grid();
        }
        y_loc--;
        update_global_grid();
        landed = true;
    }

    void left() {
        x_loc--;
        update_global_grid();
        if(!valid_state()){
            x_loc++;
            update_global_grid();
        }
    }

    void right() {
        x_loc++;
        update_global_grid();
        if(!valid_state()){
            x_loc--;
            update_global_grid();
        }
    }

    void rotate() {
        do {
            shape.rotate();
            update_global_grid();
        } while (!valid_state());
    }

    void update_global_grid() {
        int global_x, global_y, global_i;
        std::fill(global_grid.occupied.begin(), global_grid.occupied.end(), false);

        for(size_t local_i=0; local_i< shape.size; local_i++) {
            global_x = x_loc + (local_i % shape.width);
            global_y = y_loc + (local_i / shape.width);
            global_i = (global_y * sediment_grid.grid_size.x) + global_x;
            if(shape.data()[local_i]) global_grid.occupied[global_i] = true;
        }
    }

    bool valid_state() {
        for(size_t i=0; i<global_grid.n_squares; i++){
            if (global_grid.occupied[i] && sediment_grid.occupied[i]) return false;
        }
        int global_x, global_y, global_i;
        for(size_t local_i=0; local_i< shape.size; local_i++) {
            //TODO much repeated logic
            global_x = x_loc + (local_i % shape.width);
            global_y = y_loc + (local_i / shape.width);
            global_i = (global_y * sediment_grid.grid_size.x) + global_x;
            if(shape.data()[local_i]){
                if (global_i >= global_grid.n_squares) return false; // Fallen off bottom
                if (global_x < 0 || global_x >= global_grid.grid_size.x) return false ; // Off the sides
            }
        }
        return true;
    }

    Shapes::Shape shape;
    const Grid& sediment_grid;
    Grid global_grid;
    int x_loc{0};
    int y_loc{0};
    bool landed{false};
};