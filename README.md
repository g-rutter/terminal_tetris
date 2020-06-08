Tetris for the terminal, implemented in C++

# Setup
This works for me:

`clang++ -std=c++17 -lncurses -o tetris main.cpp`

If you don't have `clang++`, you might have `gcc++`.

You'll need `ncurses` installed for linking.

# Playing

Run `./tetris`.

Keys are WASD or the arrows, and space to drop the piece.

Big font size encouraged.

# My terminal is messed up after playing it.

That's an `ncurses` thing, type `reset`.