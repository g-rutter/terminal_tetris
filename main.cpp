#include <curses.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>
#include <vector>
#include <cstdio>
#include <optional>

#include "pieces.hpp"
#include "view.hpp"
#include "grid.hpp"

using milliseconds = std::chrono::duration<int, std::milli>;


class Tetris {
    public:
        Tetris(Grid& grid) : grid{grid}, tetrisview{grid} {};

        void run() {
            while(true) {
                if(!active_piece) active_piece.emplace(grid.grid_size);
                active_piece->fall();


                // if (piece landed)
                //     update occupied
                //     set active to null
                //     clean up active piece?
                //     check if game is lost
                //     check if line can be removed, 

                tetrisview.redraw(*active_piece);
                std::this_thread::sleep_for(cycle_time);
            }
        }

    private:
        const Grid grid;
        TetrisView tetrisview;        
        const milliseconds cycle_time{1000};
        std::optional<ActivePiece> active_piece{};
};


int main(){
    GridSize grid_size{10, 20};
    Grid grid(grid_size);
    Tetris tetris{grid};
    tetris.run();
}