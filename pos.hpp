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

extern std::map<uint64_t, std::string> i2s;
extern std::map<std::string, uint64_t> s2i;
extern uint64_t squares[32];

/* enum Side : int8_t { */
/*     black = -1, */
/*     white = 1, */
/* }; */

enum mType : uint8_t {
    silent,
    capture,
    promotion,
};

struct Move{
    mType type;
    uint64_t fromSquare;
    uint64_t toSquare;
    uint64_t captureSquare;

    Move(){
    }

    Move(mType m, uint64_t f, uint64_t t){
        type = m;
        fromSquare = f;
        captureSquare = 0b0;
        toSquare = t;
    }

    Move(mType m, uint64_t f, uint64_t c, uint64_t t){
        type = m;
        fromSquare = f;
        captureSquare = c;
        toSquare = t;
    }

    std::string toString(){
        return i2s[fromSquare] + "-" + i2s[toSquare];
    }

    bool operator==(const Move & m) const{
        return  (m.type == type) &&
                (m.fromSquare == fromSquare) &&
                (m.toSquare == toSquare) &&
                (m.captureSquare == captureSquare);
    }

    Move reverse();
};

struct Pos {
/* private: */
    uint64_t black;
    uint64_t white;
    uint64_t blackKing;
    uint64_t whiteKing;
    int8_t side;
    uint64_t capture;
    uint8_t fifty;

/* public: */
    Pos();
    void prettyPrint();
    std::vector<Move> genMoves();
    void playMove(Move);
    void switch_side();
    void flip_board();
     /* getSide(); */
    torch::Tensor getTensor();
    /* uint8_t getFifty(); */
    bool operator==(const Pos &) const;
};

uint64_t reverse(uint64_t);

#endif
