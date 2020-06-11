#pragma once

#include "grid.hpp"
#include "shapes.hpp"

struct ActivePiece {
    ActivePiece(const shapes::Shape shape, const Grid& sediment_grid)
        : m_shape{shape},
          m_sediment_grid{sediment_grid},
          m_global_grid{sediment_grid.m_grid_size, std::vector<bool>(sediment_grid.m_n_squares, false)},
          m_shadow_grid{sediment_grid.m_grid_size, std::vector<bool>(sediment_grid.m_n_squares, false)}
    {
        m_shape_loc.x = (m_global_grid.m_grid_size.x - shape.m_grid->m_grid_size.x) / 2;
        update_grids();
    };

    void down() {
        m_shape_loc.y++;
        if(!update_grids()){
            m_shape_loc.y--;
            update_grids();
            m_landed = true;
        }
    }

    void fall() {
        do {
            m_shape_loc.y++;
        } while (update_grids());
        m_shape_loc.y--;
        update_grids();
        m_landed = true;
    }

    void left() {
        m_shape_loc.x--;
        if(!update_grids()){
            m_shape_loc.x++;
            update_grids();
        }
    }

    void right() {
        m_shape_loc.x++;
        if(!update_grids()){
            m_shape_loc.x--;
            update_grids();
        }
    }

    void rotate() {
        int recentre_amount = (m_shape.m_grid->m_grid_size.x - m_shape.m_grid->m_grid_size.y) / 2;
        do {
            m_shape.rotate();
            m_shape_loc.x += recentre_amount;
            m_shape_loc.y -= recentre_amount;
            recentre_amount = -recentre_amount;
        } while (!update_grids());
    }

    bool update_grids() {
        // Update the global grid giving the current piece's occupancy.
        // bool return code denotes VALIDITY OF STATE.
        // Update does not complete if state is invalid.
        std::fill(m_global_grid.m_occupied.begin(), m_global_grid.m_occupied.end(), false);
        bool valid = update_grid(m_sediment_grid, m_global_grid, m_shape, m_shape_loc);

        if (valid) {
            GridCoord shadow_loc = {m_shape_loc.x, m_shape_loc.y + 1};
            while(update_grid(m_sediment_grid, m_shadow_grid, m_shape, shadow_loc)) {
                shadow_loc.y++;
            }
            shadow_loc.y--;
            std::fill(m_shadow_grid.m_occupied.begin(), m_shadow_grid.m_occupied.end(), false);
            if (shadow_loc.y - 3 > m_shape_loc.y) { // Don't show shadow if it might be visually distracting.
                update_grid(m_sediment_grid, m_shadow_grid, m_shape, shadow_loc);
            }
        } 
        return valid;
    }

    private:
        
        bool update_grid(const Grid& m_sediment_grid, Grid& update_grid, const shapes::Shape& shape, const GridCoord& shape_loc){
            GridCoord global_xy;
            size_t global_i;

            for(size_t& local_i : shape.m_grid->true_indices()){
                global_xy = shape.m_grid->to_2D(local_i) + shape_loc;
                global_i = update_grid.to_1D(global_xy);
                if (global_i >= update_grid.m_n_squares) return false; // Fallen off bottom
                else if (global_xy.x < 0 || global_xy.x >= update_grid.m_grid_size.x) return false ; // Off the sides
                else if (m_sediment_grid.m_occupied.at(global_i)) return false; // Piece overlaps with sediment (previous pieces)
                update_grid.m_occupied.at(global_i) = true;
            }
            return true;
        }
        shapes::Shape m_shape;
        const Grid& m_sediment_grid;
        GridCoord m_shape_loc{0, 0};

    public:
        bool m_landed{false};
        Grid m_global_grid, m_shadow_grid;
};