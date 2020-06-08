#pragma once
#include "active_piece.hpp"


class TetrisView {
    public:
        TetrisView(const Grid& grid) : grid{grid}
        {
            initscr();
        };

        void redraw(const std::optional<ActivePiece>& active_piece) const {
            GridCoord grid_coods;
            char ch;
            for (size_t i=0; i<grid.n_squares; i++){
                if (grid.occupied[i]) ch = '=';
                else if (active_piece.has_value() && active_piece->global_grid.occupied[i]) ch = '#';
                else ch = '.';

                grid_coods = grid.to_2D(i);
                mvwaddch(stdscr, grid_coods.y, grid_coods.x, ch);
            }
            wrefresh(stdscr);
        }

        void update_score(const int score) {
            mvprintw(grid.grid_size.y + 1, 0, "Score: %d", score);
        }

    private:
        const Grid& grid;
};