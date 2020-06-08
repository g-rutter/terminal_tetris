#pragma once
#include "active_piece.hpp"


class TetrisView {
    public:
        TetrisView(const Grid& grid, const int stretch) : grid{grid}, stretch{stretch}
        {
            initscr();
        };

        void update_grid(const std::optional<ActivePiece>& active_piece) const {
            GridCoord grid_coods;
            char ch;
            for (size_t i=0; i<grid.n_squares; i++){
                if (grid.occupied[i]) ch = '=';
                else if (active_piece.has_value() && active_piece->global_grid.occupied[i]) ch = '#';
                else ch = '.';

                grid_coods = grid.to_2D(i);

                for(size_t i=0; i<stretch; i++){ // Tile out the selected character if stretch != 1
                    for(size_t j=0; j<stretch; j++){
                        mvwaddch(stdscr, (grid_coods.y * stretch) + j,(grid_coods.x * stretch) + i, ch);
                    }
                }
            }
            wrefresh(stdscr);
        }

        void update_score(const int score, const int cycle_time_ms) const {
            mvprintw(grid.grid_size.y + 1, 0, "Score: %d", score);
            mvprintw(grid.grid_size.y + 2, 0, "Time per drop: %d ms", cycle_time_ms);
        }

        void update_highscore(const int score) const {
            mvprintw(grid.grid_size.y + 3, 0, "Highscore: %d", score);
        }

        void show_game_over() const {
            mvprintw(grid.grid_size.y, 0, "Game over: (r)estart or (q)uit");
        }

        void hide_game_over() const {
            move(grid.grid_size.y, 0);
            clrtoeol();
        }

    private:
        const Grid& grid;
        const int stretch;
};