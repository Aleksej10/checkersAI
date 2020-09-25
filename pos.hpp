#ifndef POS_H
#define POS_H

#include <iostream>
#include <cstdint>
#include <cmath>
#include <map>
#include <string>
#include <vector>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <torch/torch.h>

enum mType : uint8_t {
    silent,
    capture,
    promotion,
};

struct Move{
    mType type;
    uint64_t fromSquare;
    uint64_t captureSquare;
    uint64_t toSquare;

    Move(){}

    Move(mType m, uint64_t f, uint64_t t) :
        type(m),
        fromSquare(f),
        captureSquare(0b0),
        toSquare(t)
    {}

    Move(mType m, uint64_t f, uint64_t c, uint64_t t) :
        type(m),
        fromSquare(f),
        captureSquare(c),
        toSquare(t)
    {}


    bool operator==(const Move & m) const{
        return  (m.type == type) &&
                (m.fromSquare == fromSquare) &&
                (m.toSquare == toSquare) &&
                (m.captureSquare == captureSquare);
    }

    std::string toString();

    Move reverse();
};

struct Pos {
    uint64_t black;
    uint64_t white;
    uint64_t blackKing;
    uint64_t whiteKing;
    int8_t side;
    uint64_t capture;
    uint8_t fifty;

    Pos();
    void prettyPrint();
    std::vector<Move> * genMoves();
    void playMove(Move);
    void switch_side();
    void flip_board();
    torch::Tensor getTensor();
    bool operator==(const Pos &) const;
};

inline uint64_t reverse(uint64_t);

#endif
