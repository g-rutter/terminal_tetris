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
    Tetris(Grid& grid) : grid{grid}, tetrisview{grid} {};

    void start() {
        tetrisview.splash_screen();
        size_t i_shape = 0;
        while(getch() != ' ') {
            if(!active_piece) {
                active_piece.emplace(shapes::all_shapes[i_shape++], grid);
                i_shape %= shapes::all_shapes.size();
            }
            active_piece->down();
            active_piece->rotate();
            if (active_piece->landed) active_piece.reset();
            tetrisview.update_grid(active_piece);
            std::this_thread::sleep_for(std::chrono::milliseconds(demo_cycle_time_ms));
        }

        int highscore = 0;
        int score;
        do {
            score = run();
            highscore = score > highscore? score : highscore;
            tetrisview.update_highscore(highscore);
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
        grid.zero();
        int cycle_time_ms = start_cycle_time_ms;
        active_piece.reset();
        tetrisview.hide_game_over();

        int next_shape = shapes::random_shape();

        while(true) {
            if(!active_piece) {
                active_piece.emplace(shapes::all_shapes[next_shape], grid);
                next_shape = shapes::random_shape();

                tetrisview.update_score(score, cycle_time_ms);
                tetrisview.update_next_shape(shapes::all_shapes[next_shape]);

                if(!active_piece->update_grids()) break; // New piece being immediately invalid marks end of game.
            }
            active_piece->down();

            if (active_piece->landed){
                grid.absorb(active_piece->global_grid);
                active_piece.reset();
                score += remove_rows();
                cycle_time_ms *= 0.99;
            }

            tetrisview.update_grid(active_piece);
            if (active_piece.has_value()) input_loop(cycle_time_ms);
        }
        tetrisview.show_game_over();
        return score;
    }

    int remove_rows() {
        size_t i_low, i_high;
        int score{0};
        bool all;
        for(size_t row=0; row < grid.grid_size.y; row++) {
            i_low = row * grid.grid_size.x;
            i_high = i_low + grid.grid_size.x;
            all = std::all_of(grid.occupied.begin() + i_low, grid.occupied.begin() + i_high,
                              [](const bool v){ return v; } );
            if (all) {
                std::copy_backward(grid.occupied.begin(), grid.occupied.begin() + i_low,
                                   grid.occupied.begin() + i_high);
                std::fill(grid.occupied.begin(), grid.occupied.begin() + grid.grid_size.x, false);
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
                    active_piece->left();
                    break;
                case 'd':
                case KEY_RIGHT:
                    active_piece->right();
                    break;
                case 'w':
                case KEY_UP:
                    active_piece->rotate();
                    break;
                case 's':
                case KEY_DOWN:
                    active_piece->down();
                    take_input = !active_piece->landed;
                    break;
                case ' ':
                    active_piece->fall();
                    take_input = false;
                    break;
            }
            if(ch != ERR) tetrisview.update_grid(active_piece);
            chrono::time_point end = chrono::steady_clock::now();
            elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        } while (take_input && elapsed < cycle_time_ms);
    }

    private:
        Grid& grid;
        const TetrisView tetrisview;        
        const int start_cycle_time_ms{500};
        const int demo_cycle_time_ms{500};
        std::optional<ActivePiece> active_piece{};
};