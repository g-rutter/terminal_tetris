#pragma once
#include <array>
#include "grid.hpp"

using PieceGeo = std::array<bool, 9>;


struct Pieces {
    static const bool x = true;
    static const bool o = false;

    static constexpr PieceGeo L = {o, x, o,
                                   o, x, o,
                                   o, x, x};
    static constexpr PieceGeo T = {o, x, o,
                                   x, x, x,
                                   o, o, o};
};

struct ActivePiece {
    ActivePiece(const PieceGeo geo, const GridSize grid_size) : geo{geo}, grid_size{grid_size} {
        update_grid();
        active_grid = std::vector<unsigned short>(grid_size.x * grid_size.y, 0);
        printf("%lu\n", active_grid.size());
    };
    ActivePiece(const GridSize grid_size) : ActivePiece(Pieces::L, grid_size) {};

    void fall() {
        y_loc++;
        update_grid();
    }

    void update_grid() {
        int grid_x, grid_y, grid_i;
        std::fill(active_grid.begin(), active_grid.end(), 0);
        active_grid = std::vector<unsigned short>(grid_size.x * grid_size.y, 0); //shouldn't be needed

        for(size_t geo_i=0; geo_i< geo.size(); geo_i++) {
            grid_x = x_loc + (geo_i % 3);
            grid_y = y_loc + (geo_i / 3);
            grid_i = (grid_y * grid_size.x) + grid_x;
            if(geo[geo_i]) active_grid[grid_i] = 1;
        }
    }

    const PieceGeo geo;
    const GridSize grid_size; 
    std::vector<unsigned short> active_grid;
    int x_loc{0};
    int y_loc{0};
};