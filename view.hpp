#pragma once
#include "active_piece.hpp"


class TetrisView {
    public:
        TetrisView(const Grid& grid, const int stretch) : grid{grid}, stretch{stretch}
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
                for(size_t i=0; i<stretch; i++){
                    for(size_t j=0; j<stretch; j++){
                        mvwaddch(stdscr, (grid_coods.y * stretch) + j,(grid_coods.x * stretch) + i, ch);
                    }
                }
            }
            wrefresh(stdscr);
        }

        void update_score(const int score, const int cycle_time_ms) const {
            mvprintw(grid.grid_size.y + 1, 0, "Score: %d", score);
            mvprintw(grid.grid_size.y + 2, 0, "Falls every: %d ms", cycle_time_ms);
        }

    private:
        const Grid& grid;
        const int stretch;
};