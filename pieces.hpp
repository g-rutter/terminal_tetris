#include <array>

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

struct Piece {
    Piece(PieceGeo geo) : geo{geo} {};
    Piece() : geo{Pieces::L} {};
    // operator=
    // Piece& operator=(const Piece &) {

    // };

    const PieceGeo geo;
};