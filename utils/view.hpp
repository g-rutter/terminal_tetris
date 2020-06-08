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
                if (grid.occupied[i]) ch = '=';
                else if (active_piece.has_value() && active_piece->global_grid.occupied[i]) ch = '#';
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
            int text_y = 2;
            int shape_y = text_y + 2;

            mvprintw(text_y, grid.grid_size.x + 1, "Next up:");
            for(int i=0; i<5; i++) {
                move(shape_y + i, grid.grid_size.x + 1);
                clrtoeol();
            }
            int x, y;
            auto& shape_vector = shape.data();
            for (size_t i=0; i<shape_vector.size(); i++){
                x = grid.grid_size.x + 3 + (i % shape.width);
                y = shape_y + (i / shape.width);
                if (shape_vector[i]) mvwaddch(stdscr, y, x, '#');
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