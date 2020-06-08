#pragma once
#include "pieces.hpp"


class TetrisView {
    public:
        TetrisView(const Grid& grid) : grid{grid}
        {
            initscr();
        };

        void redraw(const ActivePiece& active_piece) const {
            int x, y;
            char ch;
            for (size_t i=0; i<grid.n_squares; i++){
                x = i % grid.grid_size.x;
                y = i / grid.grid_size.x;
                ch = grid.sediment[i] ? 'x' : 'o';
                mvwaddch(stdscr, x, y, ch);
            }
            wrefresh(stdscr);
        }    

    private:
        const Grid& grid;
};