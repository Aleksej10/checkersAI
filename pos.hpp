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

enum Side : int8_t {
    black = -1,
    white = 1,
};

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
};

class Pos {
private:
    uint64_t black;
    uint64_t white;
    uint64_t blackKing;
    uint64_t whiteKing;
    Side side;
    uint64_t capture;
    uint8_t b_n;
    uint8_t w_n;
    uint8_t bK_n;
    uint8_t wK_n;
    uint8_t fifty;
public:
    Pos();
    void prettyPrint();
    std::vector<Move> genMoves();
    void playMove(Move);
    void switch_side();
    void flip_board();
    void playString(const std::string);
    bool is_one_v_one();
    float one_v_one();
    float proximity();
    Side getSide();
    std::vector<uint8_t> serialize();
    torch::Tensor getTensor();
    uint8_t getFifty();
};

void printMoves(const std::vector<Move>&);
uint64_t reverse(uint64_t);
void printBits(uint64_t);

#endif
