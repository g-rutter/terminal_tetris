#pragma once

#include <curses.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>
#include <vector>
#include <cstdio>
#include <optional>
#include <algorithm>

#include "active_piece.hpp"
#include "view.hpp"
#include "grid.hpp"
#include "shapes.hpp"

using namespace std;

struct Tetris {
    Tetris(Grid& grid) : m_grid{grid}, m_tetrisview{grid} {};

    void start() {
        m_tetrisview.splash_screen();
        size_t i_shape = 0;
        while(getch() != ' ') {
            if(!m_active_piece) {
                m_active_piece.emplace(shapes::all_shapes[i_shape++], m_grid);
                i_shape %= shapes::all_shapes.size();
            }
            m_active_piece->down();
            m_active_piece->rotate();
            if (m_active_piece->m_landed) m_active_piece.reset();
            m_tetrisview.update_gridview(m_active_piece);
            std::this_thread::sleep_for(std::chrono::milliseconds(m_demo_cycle_time_ms));
        }

        int highscore = 0;
        int score;
        do {
            score = run();
            highscore = score > highscore? score : highscore;
            m_tetrisview.update_highscore(highscore);
        } while(restart_input());
    }

    bool restart_input() {
        nodelay(stdscr, FALSE);
        while(true) {
            switch (getch()){
                case 'r':
                    return true;
                case 'q':
                    return false;
            }
        }
    }

    int run() {
        int score = 0;
        m_grid.zero();
        int cycle_time_ms = m_start_cycle_time_ms;
        m_active_piece.reset();
        m_tetrisview.hide_game_over();

        int next_shape = shapes::random_shape();

        while(true) {
            if(!m_active_piece) {
                m_active_piece.emplace(shapes::all_shapes[next_shape], m_grid);
                next_shape = shapes::random_shape();

                m_tetrisview.update_score(score, cycle_time_ms);
                m_tetrisview.update_next_shape(shapes::all_shapes[next_shape]);

                if(!m_active_piece->update_grids()) break; // New piece being immediately invalid marks end of game.
            }
            m_active_piece->down();

            if (m_active_piece->m_landed){
                m_grid.absorb(m_active_piece->m_global_grid);
                m_active_piece.reset();
                score += remove_rows();
                cycle_time_ms *= 0.99;
            }

            m_tetrisview.update_gridview(m_active_piece);
            if (m_active_piece.has_value()) input_loop(cycle_time_ms);
        }
        m_tetrisview.show_game_over();
        return score;
    }

    int remove_rows() {
        size_t i_low, i_high;
        int score{0};
        bool all;
        for(size_t row=0; row < m_grid.m_grid_size.y; row++) {
            i_low = row * m_grid.m_grid_size.x;
            i_high = i_low + m_grid.m_grid_size.x;
            all = std::all_of(m_grid.m_occupied.begin() + i_low, m_grid.m_occupied.begin() + i_high,
                              [](const bool v){ return v; } );
            if (all) {
                std::copy_backward(m_grid.m_occupied.begin(), m_grid.m_occupied.begin() + i_low,
                                   m_grid.m_occupied.begin() + i_high);
                std::fill(m_grid.m_occupied.begin(), m_grid.m_occupied.begin() + m_grid.m_grid_size.x, false);
                score++;
            }
        }
        if(score == 4) score = 8; // Bonus for getting tetris
        return score;
    }

    void input_loop(int cycle_time_ms) {
        const chrono::time_point start = chrono::steady_clock::now();
        int elapsed;
        bool take_input = true;
        do {
            int ch = getch();
            switch (ch) {
                case 'a':
                case KEY_LEFT:
                    m_active_piece->left();
                    break;
                case 'd':
                case KEY_RIGHT:
                    m_active_piece->right();
                    break;
                case 'w':
                case KEY_UP:
                    m_active_piece->rotate();
                    break;
                case 's':
                case KEY_DOWN:
                    m_active_piece->down();
                    take_input = !m_active_piece->m_landed;
                    break;
                case ' ':
                    m_active_piece->fall();
                    take_input = false;
                    break;
            }
            if(ch != ERR) m_tetrisview.update_gridview(m_active_piece);
            chrono::time_point end = chrono::steady_clock::now();
            elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        } while (take_input && elapsed < cycle_time_ms);
    }

    private:
        Grid& m_grid;
        const TetrisView m_tetrisview;        
        const int m_start_cycle_time_ms{500};
        const int m_demo_cycle_time_ms{500};
        std::optional<ActivePiece> m_active_piece{};
};