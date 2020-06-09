Tetris for the terminal, implemented in C++

![alt text](screenshot.png)

# Setup
This works for me:

`clang++ -std=c++17 -lncurses -o tetris main.cpp`

If you don't have `clang++`, you might have `g++`.

You'll need `ncurses` installed for linking.

# Playing

Run `./tetris`.

Keys are WASD or the arrows, and space to drop the piece.

Big font size encouraged.

# My terminal is messed up after playing it.

That's an `ncurses` thing, type `reset`.

# To do

* Command-line options: Start speed, grid size, (whether to show the next piece), speedup rate.
* Proper scoring?
* Colour?
* Linting/whitespace rules
* Package layout
* Add that thing from real tetris where you have a chance to make diagonal moves, where you shouldn't really be able to fit in. If I remember correctly.