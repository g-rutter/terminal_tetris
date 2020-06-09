#pragma once
#include "active_piece.hpp"


class TetrisView {
    public:
        TetrisView(const Grid& grid) : grid{grid}
        {
            initscr();
        };

        void update_grid(const std::optional<ActivePiece>& active_piece) const {
            GridCoord grid_coods;
            char ch;
            for (size_t i=0; i<grid.n_squares; i++){
                if (grid.occupied.at(i)) ch = '=';
                else if (active_piece.has_value() && active_piece->global_grid.occupied.at(i)) ch = '#';
                else ch = '.';

                grid_coods = grid.to_2D(i);

                mvwaddch(stdscr, grid_coods.y, grid_coods.x, ch);
            }
            wrefresh(stdscr);
        }

        void update_score(const int score, const int cycle_time_ms) const {
            mvprintw(grid.grid_size.y + 1, 0, "Score: %d", score);
            mvprintw(grid.grid_size.y + 2, 0, "Drop every: %d ms", cycle_time_ms);
        }

        void update_highscore(const int score) const {
            mvprintw(grid.grid_size.y + 3, 0, "Highscore: %d", score);
        }

        void update_next_shape(const Shapes::Shape& shape) const {
            const GridCoord text_loc{grid.grid_size.x + 1, 2};
            const GridCoord shape_loc{text_loc.x + 2, text_loc.y + 2};

            mvprintw(text_loc.y, text_loc.x, "Next up:");
            for(size_t i=0; i<5; i++) {
                move(shape_loc.y + i, grid.grid_size.x + 1);
                clrtoeol();
            }

            GridCoord global_coords;
            const auto& shape_grid = *shape.grid;
            for (auto &&i : shape_grid.true_indices()){
                global_coords = shape_grid.to_2D(i) + shape_loc;
                mvwaddch(stdscr, global_coords.y, global_coords.x, '#');
            }
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
};