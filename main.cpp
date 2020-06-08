#include <curses.h>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>
#include <vector>
#include <cstdio>

#include "pieces.hpp"
#include "view.hpp"

using milliseconds = std::chrono::duration<int, std::milli>;


class Tetris {
    public:
        Tetris() : Tetris(GridSize{5, 10}) {};
        Tetris(const GridSize grid_size) : grid_size{grid_size}, tetrisview{grid_size} {
            occupied = std::vector<bool>(grid_size.y * grid_size.x, false);
        };

        void run() {


            while(true) {
                if(!active_piece) {
                    // Create a new active piece
                }
                // move piece down
                // 
                // if (piece landed)
                //     update occupied
                //     set active to null
                //     clean up active piece?
                //     check if game is lost
                //     check if line can be removed, 

                tetrisview.redraw(occupied, active);
                std::this_thread::sleep_for(cycle_time);
            }
        }

    private:
        const GridSize grid_size;
        TetrisView tetrisview;
        // std::vector<Tetra> sediment;
        std::vector<bool> occupied;
        const milliseconds cycle_time{1000};
        std::optional<Piece> active_piece{};
        Piece active2_piece{};
};


int main(){
    Tetris tetris{};
    tetris.run();
}