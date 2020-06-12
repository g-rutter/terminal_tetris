#pragma once

#include <curses.h>
#include <thread>
#include <chrono>
#include <vector>
#include <cstdio>
#include <optional>
#include <algorithm>

#include "active_piece.hpp"
#include "view.hpp"
#include "grid.hpp"
#include "shapes.hpp"
#include "input.hpp"

using namespace std;

struct Tetris {
    Tetris(Grid& grid) : m_grid{grid},
                         m_tetrisview{grid},
                         m_active_piece{&shapes::L, m_grid} {};

    void start() {
        splash();
        
        InputResult restart_result;
        InputManager<InputMode::Restart> restart_input_manager{m_tetrisview, m_active_piece};

        int highscore = 0;
        int score;
        do {
            score = play();
            highscore = score > highscore? score : highscore;
            m_tetrisview.update_highscore(highscore);
            restart_result = restart_input_manager.input_loop(std::numeric_limits<int>::max());
        } while(restart_result != InputResult::EndGame);
    }

    void splash() {
        m_tetrisview.splash_screen();
        size_t i_shape = 1;
        InputResult splash_result;
        InputManager<InputMode::SplashScreen> splash_input_manager{m_tetrisview, m_active_piece};

        do {
            if(m_active_piece.m_landed) {
                m_active_piece.reset(&shapes::all_shapes[i_shape++]);
                i_shape %= shapes::all_shapes.size();
            }
            m_tetrisview.update_gridview(m_active_piece);
            splash_result = splash_input_manager.input_loop(m_start_cycle_time_ms);
            m_active_piece.down();
            m_active_piece.rotate();
        } while (splash_result == InputResult::Continue);
    }

    int play() {
        int score = 0;
        m_grid.zero();
        int cycle_time_ms = m_start_cycle_time_ms;
        m_tetrisview.hide_game_over();

        m_active_piece.reset(&shapes::all_shapes[0]);
        int next_shape = shapes::random_shape();
        InputManager<InputMode::Play> input_manager{m_tetrisview, m_active_piece};

        m_tetrisview.update_score(score, cycle_time_ms);
        m_tetrisview.update_next_shape(shapes::all_shapes[next_shape]);

        while(true) {
            if(m_active_piece.m_landed) {
                m_grid.absorb(m_active_piece.m_global_grid);
                score += remove_rows();
                cycle_time_ms *= 0.99;

                m_active_piece.reset(&shapes::all_shapes[next_shape]);
                next_shape = shapes::random_shape();

                if(!m_active_piece.update_grids()) break; // New piece being immediately invalid marks end of game.
                m_tetrisview.update_score(score, cycle_time_ms);
                m_tetrisview.update_next_shape(shapes::all_shapes[next_shape]);
            }
            m_tetrisview.update_gridview(m_active_piece);
            input_manager.input_loop(cycle_time_ms);
            m_active_piece.down();
        }
        m_tetrisview.update_gridview(m_active_piece); // Show end "collision" state as final view of game.
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

    private:
        Grid& m_grid;
        const TetrisView m_tetrisview;        
        const int m_start_cycle_time_ms{500};
        ActivePiece m_active_piece;
};