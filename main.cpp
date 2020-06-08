#include "grid.hpp"
#include "tetris.hpp"

int main(){
    GridSize grid_size{10, 15};
    Grid grid(grid_size);
    Tetris tetris{grid};
    tetris.start();
}