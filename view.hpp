
struct GridSize{
    int x;
    int y;
};

class TetrisView {
    public:
        TetrisView(const GridSize grid_size) : grid_size{grid_size}, n_squares{grid_size.x * grid_size.y}
        {
            initscr();
        };

        void redraw(const std::vector<bool>& occupied) const {
            int x, y;
            char ch;
            for (size_t i=0; i<n_squares; i++){
                x = i % grid_size.x;
                y = i / grid_size.x;
                ch = occupied[i] ? 'x' : 'o';
                mvwaddch(stdscr, x, y, ch);
            }
            wrefresh(stdscr);
        }    

    private:
        const GridSize grid_size;
        const int n_squares;
};