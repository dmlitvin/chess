//
// Created by Dmitro LITVINENKO on 2019-11-10.
//

#ifndef CHESS_CHESSFIGURE_HPP
#define CHESS_CHESSFIGURE_HPP


struct ChessFigure {

    enum class figure_type {
        pawn,
        rook,
        knight,
        bishop,
        queen,
        king
    };

    enum class figure_team {
        white,
        black
    };

    figure_type type;
    figure_team team;

};


#endif //CHESS_CHESSFIGURE_HPP
