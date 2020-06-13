#pragma once

#include "grid.hpp"
#include "shapes.hpp"

struct ActivePiece {
    ActivePiece(const shapes::Shape* shape, const Grid& sediment_grid)
        : m_shape{shape},
          m_sediment_grid{sediment_grid},
          m_global_grid{sediment_grid.m_grid_size, std::vector<bool>(sediment_grid.m_n_squares, false)},
          m_shadow_grid{sediment_grid.m_grid_size, std::vector<bool>(sediment_grid.m_n_squares, false)}
    {
        reset(m_shape);
    };

    void reset(const shapes::Shape* shape) { // Feed a new shape in to be the active piece.
        m_shape = shape;
        m_rotation = 0;
        m_grid = &shape->m_grids[0];
        m_shape_loc.y = 0;
        m_shape_loc.x = (m_global_grid.m_grid_size.x - m_grid->m_grid_size.x) / 2;
        m_landed = false;
        m_recentre_amount = (m_grid->m_grid_size.x - m_grid->m_grid_size.y) / 2;
        update_grids();
    }

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
        do {
            m_rotation ++;
            m_rotation %= 4;
            m_grid = &m_shape->m_grids[m_rotation];
            if (m_rotation % 2) {
                m_shape_loc.x += m_recentre_amount;
                m_shape_loc.y -= m_recentre_amount;
            } else {
                m_shape_loc.x -= m_recentre_amount;
                m_shape_loc.y += m_recentre_amount;
            }
        } while (!update_grids());
    }

    bool update_grids() {
        // Update the global grid giving the current piece's occupancy.
        // bool return code denotes VALIDITY OF STATE.
        // Update does not complete if state is invalid.
        std::fill(m_global_grid.m_occupied.begin(), m_global_grid.m_occupied.end(), false);
        bool valid = update_grid(m_sediment_grid, m_global_grid, m_shape_loc);

        if (valid) {
            GridCoord shadow_loc = {m_shape_loc.x, m_shape_loc.y + 1};
            while(update_grid(m_sediment_grid, m_shadow_grid, shadow_loc)) {
                shadow_loc.y++;
            }
            shadow_loc.y--;
            std::fill(m_shadow_grid.m_occupied.begin(), m_shadow_grid.m_occupied.end(), false);
            if (shadow_loc.y - 3 > m_shape_loc.y) { // Don't show shadow if it might be visually distracting.
                update_grid(m_sediment_grid, m_shadow_grid, shadow_loc);
            }
        } 
        return valid;
    }

    private:
        bool update_grid(const Grid& m_sediment_grid, Grid& update_grid, const GridCoord& shape_loc){
            GridCoord global_xy;
            size_t global_i;

            for(size_t& local_i : m_grid->true_indices()){
                global_xy = m_grid->to_2D(local_i) + shape_loc;
                global_i = update_grid.to_1D(global_xy);
                if (global_i >= update_grid.m_n_squares) return false; // Fallen off bottom
                else if (global_xy.x < 0 || global_xy.x >= update_grid.m_grid_size.x) return false ; // Off the sides
                else if (m_sediment_grid.m_occupied.at(global_i)) return false; // Piece overlaps with sediment (previous pieces)
                update_grid.m_occupied.at(global_i) = true;
            }
            return true;
        }
        const shapes::Shape* m_shape;
        const Grid& m_sediment_grid;
        GridCoord m_shape_loc{0, 0};
        int m_rotation = 0;
        int m_recentre_amount = 0;

    public:
        bool m_landed{false};
        const Grid* m_grid;
        Grid m_global_grid, m_shadow_grid;
};