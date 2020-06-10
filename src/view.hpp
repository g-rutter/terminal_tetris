#pragma once
#include "active_piece.hpp"


class TetrisView {
    public:
        TetrisView(const Grid& grid) : m_grid{grid}
        {
            initscr();
            curs_set(0); // hide cursor
            nodelay(stdscr, TRUE);
            keypad(stdscr, TRUE);
        };

        void splash_screen() const {
            const GridCoord text_loc{m_grid.m_grid_size.x + 1, 2};
            write_line("Controls", text_loc.x, text_loc.y, A_NORMAL);
            write_line("wasd/arrows - move", text_loc.x, text_loc.y + 2, A_NORMAL);
            write_line("w/up - rotate", text_loc.x, text_loc.y + 3, A_NORMAL);
            write_line("space - drop piece", text_loc.x, text_loc.y + 4, A_NORMAL);
            write_line("PRESS SPACE TO START", text_loc.x, text_loc.y + 6, A_UNDERLINE);
            update_grid(std::nullopt);
            wrefresh(stdscr);
        }

        void update_grid(const std::optional<ActivePiece>& active_piece) const {
            GridCoord grid_coords;
            char ch;
            size_t effect;
            bool is_border;
            for (size_t i=0; i<m_grid.m_n_squares; i++){
                effect = A_NORMAL;
                grid_coords = m_grid.to_2D(i);
                if (m_grid.m_occupied.at(i)) {
                    effect = A_REVERSE;
                    ch = ' ';
                }
                else if (active_piece.has_value() && active_piece->m_global_grid.m_occupied.at(i)) ch = '#';
                else if (active_piece.has_value() && active_piece->m_shadow_grid.m_occupied.at(i)) ch = 'x';
                else {
                    is_border = (grid_coords.x == 0
                                 || grid_coords.x + 1 == m_grid.m_grid_size.x
                                 || grid_coords.y + 1 == m_grid.m_grid_size.y);
                    effect = is_border ? A_NORMAL : A_INVIS;
                    ch = '.';
                }
                write_char(ch, grid_coords.x, grid_coords.y, effect);
            }
            wrefresh(stdscr);
        }

        void update_score(const int score, const int cycle_time_ms) const {
            clear_line(0, m_grid.m_grid_size.y + 1);
            clear_line(0, m_grid.m_grid_size.y + 2);
            char score_str[100];
            sprintf(score_str, "Score: %d\nDrop every: %d ms", score, cycle_time_ms);
            write_line(score_str, 0, m_grid.m_grid_size.y + 1, A_NORMAL);
        }

        void update_highscore(const int score) const {
            char highscore_str[100];
            sprintf(highscore_str, "Highscore: %d", score);
            write_line(highscore_str, 0, m_grid.m_grid_size.y + 3, A_NORMAL);
        }

        void update_next_shape(const shapes::Shape& shape) const {
            const GridCoord text_loc{m_grid.m_grid_size.x + 1, 2};
            const GridCoord m_shape_loc{text_loc.x + 2, text_loc.y + 2};

            write_line("Next up:", text_loc.x, text_loc.y, A_NORMAL);
            for(size_t i=0; i<5; i++) clear_line(m_grid.m_grid_size.x + 1, m_shape_loc.y + i);

            GridCoord global_coords;
            const auto& shape_grid = *shape.m_grid;
            for (auto &&i : shape_grid.true_indices()){
                global_coords = shape_grid.to_2D(i) + m_shape_loc;
                write_char('#', global_coords.x, global_coords.y, A_NORMAL);
            }
        }

        void show_game_over() const {
            write_line("Game over: (r)estart or (q)uit", 0, m_grid.m_grid_size.y, A_BOLD);
        }

        void hide_game_over() const {
            clear_line(0, m_grid.m_grid_size.y);
            write_line("", 0, m_grid.m_grid_size.y, A_NORMAL);
        }

        void write_char(const char ch, const int x, const int y, const size_t effect) const {
            attroff(A_INVIS);
            attron(effect);
            mvwaddch(stdscr, y, x, ch);
            attroff(effect);
            attron(A_INVIS); // Stop user input appearing
        }

        void clear_line(const int x, const int y) const {
            move(y, x);
            clrtoeol();
        }

        void write_line(const char* str, const int x, const int y, const size_t effect) const {
            attroff(A_INVIS);
            attron(effect);
            mvprintw(y, x, str);
            attroff(effect);
            attron(A_INVIS); // Stop user input appearing
        }

    private:
        const Grid& m_grid;
};