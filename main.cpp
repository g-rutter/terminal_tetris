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

using namespace std;
using milliseconds = std::chrono::duration<int, std::milli>;

class Tetris {
    public:
        Tetris(Grid& grid) : grid{grid}, tetrisview{grid} {};

        void run() {
            int score = 0;

            while(true) {
                if(!active_piece) {
                    active_piece.emplace(grid);
                    tetrisview.update_score(score);
                    if(!active_piece->valid_state()) break;
                }
                active_piece->down();

                if (active_piece->landed){
                    grid.absorb(active_piece->global_grid);
                    active_piece.reset();
                    score += remove_rows();
                }

                tetrisview.redraw(active_piece);
                if (active_piece.has_value()) input_loop();
            }
            printf("Score: %d", score);
            printf("\nGame over.");
        }


    int remove_rows() {
        size_t i_low, i_high;
        int score{0};
        bool all;
        for(size_t row=0; row < grid.grid_size.y; row++) {
            i_low = row * grid.grid_size.x;
            i_high = i_low + grid.grid_size.x - 1;
            all = std::all_of(std::begin(grid.occupied)+i_low, std::begin(grid.occupied)+i_high,
                              [](const bool v){ return v; } );
            if (all) {
                std::copy_backward(grid.occupied.begin(), grid.occupied.begin() + i_low - 1,
                                   grid.occupied.begin() + i_high);
                std::fill(grid.occupied.begin(), grid.occupied.begin() + grid.grid_size.x, false);
                score++;
            }
        }
        return score;
    }

        void input_loop() {
            const chrono::time_point start = chrono::steady_clock::now();
            int elapsed;
            bool take_input = true;
            nodelay(stdscr, TRUE);
            keypad(stdscr, TRUE);
            do {
                switch (getch()) {
                    case 'a':
                    case KEY_LEFT:
                        active_piece->left();
                        tetrisview.redraw(active_piece);
                        break;
                    case 'd':
                    case KEY_RIGHT:
                        active_piece->right();
                        tetrisview.redraw(active_piece);
                        break;
                    case 'w':
                    case KEY_UP:
                        active_piece->rotate();
                        tetrisview.redraw(active_piece);
                        break;
                    case 's':
                    case KEY_DOWN:
                        active_piece->down();
                        tetrisview.redraw(active_piece);
                        take_input = !active_piece->landed;
                        break;
                    case ' ':
                        active_piece->fall();
                        tetrisview.redraw(active_piece);
                        take_input = false;
                        break;
                }
                chrono::time_point end = chrono::steady_clock::now();
                elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
            } while (take_input && elapsed < cycle_time_ms);
        }

    private:
        Grid& grid;
        TetrisView tetrisview;        
        const int cycle_time_ms{500};
        std::optional<ActivePiece> active_piece{};
};


int main(){
    GridSize grid_size{10, 15};
    Grid grid(grid_size);
    Tetris tetris{grid};
    tetris.run();
}