#include "pos.hpp"

uint64_t squares[32] = {
0b0000000000000000000000000000000000000000000000000000000000000010,
0b0000000000000000000000000000000000000000000000000000000000001000,
0b0000000000000000000000000000000000000000000000000000000000100000,
0b0000000000000000000000000000000000000000000000000000000010000000,
0b0000000000000000000000000000000000000000000000000000000100000000,
0b0000000000000000000000000000000000000000000000000000010000000000,
0b0000000000000000000000000000000000000000000000000001000000000000,
0b0000000000000000000000000000000000000000000000000100000000000000,
0b0000000000000000000000000000000000000000000000100000000000000000,
0b0000000000000000000000000000000000000000000010000000000000000000,
0b0000000000000000000000000000000000000000001000000000000000000000,
0b0000000000000000000000000000000000000000100000000000000000000000,
0b0000000000000000000000000000000000000001000000000000000000000000,
0b0000000000000000000000000000000000000100000000000000000000000000,
0b0000000000000000000000000000000000010000000000000000000000000000,
0b0000000000000000000000000000000001000000000000000000000000000000,
0b0000000000000000000000000000001000000000000000000000000000000000,
0b0000000000000000000000000000100000000000000000000000000000000000,
0b0000000000000000000000000010000000000000000000000000000000000000,
0b0000000000000000000000001000000000000000000000000000000000000000,
0b0000000000000000000000010000000000000000000000000000000000000000,
0b0000000000000000000001000000000000000000000000000000000000000000,
0b0000000000000000000100000000000000000000000000000000000000000000,
0b0000000000000000010000000000000000000000000000000000000000000000,
0b0000000000000010000000000000000000000000000000000000000000000000,
0b0000000000001000000000000000000000000000000000000000000000000000,
0b0000000000100000000000000000000000000000000000000000000000000000,
0b0000000010000000000000000000000000000000000000000000000000000000,
0b0000000100000000000000000000000000000000000000000000000000000000,
0b0000010000000000000000000000000000000000000000000000000000000000,
0b0001000000000000000000000000000000000000000000000000000000000000,
0b0100000000000000000000000000000000000000000000000000000000000000};

std::map<uint64_t, std::string> i2s = {
{0b0000000000000000000000000000000000000000000000000000000000000000,   ""},
{0b0101010110101010010101011010101001010101101010100101010110101010, "lg"},
{0b0000000000000000000000000000000000000000000000000000000000000010, "g1"},
{0b0000000000000000000000000000000000000000000000000000000000001000, "e1"},
{0b0000000000000000000000000000000000000000000000000000000000100000, "c1"},
{0b0000000000000000000000000000000000000000000000000000000010000000, "a1"},
{0b0000000000000000000000000000000000000000000000000000000100000000, "h2"},
{0b0000000000000000000000000000000000000000000000000000010000000000, "f2"},
{0b0000000000000000000000000000000000000000000000000001000000000000, "d2"},
{0b0000000000000000000000000000000000000000000000000100000000000000, "b2"},
{0b0000000000000000000000000000000000000000000000100000000000000000, "g3"},
{0b0000000000000000000000000000000000000000000010000000000000000000, "e3"},
{0b0000000000000000000000000000000000000000001000000000000000000000, "c3"},
{0b0000000000000000000000000000000000000000100000000000000000000000, "a3"},
{0b0000000000000000000000000000000000000001000000000000000000000000, "h4"},
{0b0000000000000000000000000000000000000100000000000000000000000000, "f4"},
{0b0000000000000000000000000000000000010000000000000000000000000000, "d4"},
{0b0000000000000000000000000000000001000000000000000000000000000000, "b4"},
{0b0000000000000000000000000000001000000000000000000000000000000000, "g5"},
{0b0000000000000000000000000000100000000000000000000000000000000000, "e5"},
{0b0000000000000000000000000010000000000000000000000000000000000000, "c5"},
{0b0000000000000000000000001000000000000000000000000000000000000000, "a5"},
{0b0000000000000000000000010000000000000000000000000000000000000000, "h6"},
{0b0000000000000000000001000000000000000000000000000000000000000000, "f6"},
{0b0000000000000000000100000000000000000000000000000000000000000000, "d6"},
{0b0000000000000000010000000000000000000000000000000000000000000000, "b6"},
{0b0000000000000010000000000000000000000000000000000000000000000000, "g7"},
{0b0000000000001000000000000000000000000000000000000000000000000000, "e7"},
{0b0000000000100000000000000000000000000000000000000000000000000000, "c7"},
{0b0000000010000000000000000000000000000000000000000000000000000000, "a7"},
{0b0000000100000000000000000000000000000000000000000000000000000000, "h8"},
{0b0000010000000000000000000000000000000000000000000000000000000000, "f8"},
{0b0001000000000000000000000000000000000000000000000000000000000000, "d8"},
{0b0100000000000000000000000000000000000000000000000000000000000000, "b8"}};

std::map<std::string, uint64_t> s2i = {
{""  , 0b0000000000000000000000000000000000000000000000000000000000000000},
{"lg", 0b0101010110101010010101011010101001010101101010100101010110101010},
{"g1", 0b0000000000000000000000000000000000000000000000000000000000000010},
{"e1", 0b0000000000000000000000000000000000000000000000000000000000001000},
{"c1", 0b0000000000000000000000000000000000000000000000000000000000100000},
{"a1", 0b0000000000000000000000000000000000000000000000000000000010000000},
{"h2", 0b0000000000000000000000000000000000000000000000000000000100000000},
{"f2", 0b0000000000000000000000000000000000000000000000000000010000000000},
{"d2", 0b0000000000000000000000000000000000000000000000000001000000000000},
{"b2", 0b0000000000000000000000000000000000000000000000000100000000000000},
{"g3", 0b0000000000000000000000000000000000000000000000100000000000000000},
{"e3", 0b0000000000000000000000000000000000000000000010000000000000000000},
{"c3", 0b0000000000000000000000000000000000000000001000000000000000000000},
{"a3", 0b0000000000000000000000000000000000000000100000000000000000000000},
{"h4", 0b0000000000000000000000000000000000000001000000000000000000000000},
{"f4", 0b0000000000000000000000000000000000000100000000000000000000000000},
{"d4", 0b0000000000000000000000000000000000010000000000000000000000000000},
{"b4", 0b0000000000000000000000000000000001000000000000000000000000000000},
{"g5", 0b0000000000000000000000000000001000000000000000000000000000000000},
{"e5", 0b0000000000000000000000000000100000000000000000000000000000000000},
{"c5", 0b0000000000000000000000000010000000000000000000000000000000000000},
{"a5", 0b0000000000000000000000001000000000000000000000000000000000000000},
{"h6", 0b0000000000000000000000010000000000000000000000000000000000000000},
{"f6", 0b0000000000000000000001000000000000000000000000000000000000000000},
{"d6", 0b0000000000000000000100000000000000000000000000000000000000000000},
{"b6", 0b0000000000000000010000000000000000000000000000000000000000000000},
{"g7", 0b0000000000000010000000000000000000000000000000000000000000000000},
{"e7", 0b0000000000001000000000000000000000000000000000000000000000000000},
{"c7", 0b0000000000100000000000000000000000000000000000000000000000000000},
{"a7", 0b0000000010000000000000000000000000000000000000000000000000000000},
{"h8", 0b0000000100000000000000000000000000000000000000000000000000000000},
{"f8", 0b0000010000000000000000000000000000000000000000000000000000000000},
{"d8", 0b0001000000000000000000000000000000000000000000000000000000000000},
{"b8", 0b0100000000000000000000000000000000000000000000000000000000000000}};

static const uint64_t first_1    =  0b0000000010101010010101011010101001010101101010100101010110101010;
static const uint64_t r_first_1  =  0b0101010110101010010101011010101001010101101010100101010100000000;
static const uint64_t first_2    =  0b0101010110101010000000000000000000000000000000000000000000000000;
static const uint64_t n_first_2  =  0b0000000000000000010101011010101001010101101010100101010110101010;
static const uint64_t r_first_2  =  0b0101010110101010010101011010101001010101101010100000000000000000;
static const uint64_t first_3    =  0b0101010110101010010101010000000000000000000000000000000000000000;
static const uint64_t n_first_3  =  0b0000000000000000000000001010101001010101101010100101010110101010;
static const uint64_t legal_mask =  0b0101010110101010010101011010101001010101101010100101010110101010;

static const uint64_t b_mak      =  0b0000000010101010000000001010101000000000101010100000000010101010;

uint64_t reverse(uint64_t n){
    n = ((n >> 1) & 0b0101010101010101010101010101010101010101010101010101010101010101)|
        ((n << 1) & 0b1010101010101010101010101010101010101010101010101010101010101010);
    n = ((n >> 2) & 0b0011001100110011001100110011001100110011001100110011001100110011)|
        ((n << 2) & 0b1100110011001100110011001100110011001100110011001100110011001100);
    n = ((n >> 4) & 0b0000111100001111000011110000111100001111000011110000111100001111)|
        ((n << 4) & 0b1111000011110000111100001111000011110000111100001111000011110000);
    n = ((n >> 8) & 0b0000000011111111000000001111111100000000111111110000000011111111)|
        ((n << 8) & 0b1111111100000000111111110000000011111111000000001111111100000000);
    n = ((n >>16) & 0b0000000000000000111111111111111100000000000000001111111111111111)|
        ((n <<16) & 0b1111111111111111000000000000000011111111111111110000000000000000);
    n = ((n >>32) & 0b0000000000000000000000000000000011111111111111111111111111111111)|
        ((n <<32) & 0b1111111111111111111111111111111100000000000000000000000000000000);
    return n;
}

void printMoves(const std::vector<Move>& moves){
    for(long unsigned i=0; i<moves.size(); i++){
        std::cout << (i+1) << ". "
                  << i2s[moves[i].fromSquare] << "-"
                  << i2s[moves[i].toSquare] << " c: "
                  << i2s[moves[i].captureSquare] << " ";
        if (moves[i].type == mType::silent)
            std::cout << "silent\n";
        else if(moves[i].type == mType::promotion)
            std::cout << "prootion\n";
        else
            std::cout << '\n';
    }
}

void printBits(uint64_t n){
    uint64_t mask = 0b1000000000000000000000000000000000000000000000000000000000000000;
    for(uint64_t i = 0; i<64; i++){
        if((mask & n) != 0)
            std::cout << "1 ";
        else
            std::cout << "0 ";
        if((i+1)%8 == 0)
            std::cout << '\n';
        mask >>= 1;
    }
    std::cout << '\n';
}

Pos::Pos(){
    white = 0b0101010110101010010101010000000000000000000000000000000000000000;
    black = 0b0000000000000000000000000000000000000000101010100101010110101010;
    whiteKing = 0b0;
    blackKing = 0b0;
    side = Side::black;
    capture = legal_mask;
    b_n = 12;
    w_n = 12;
    bK_n = 0;
    wK_n = 0;
    fifty = 0;
}

void Pos::prettyPrint(){
    /* system("clear"); */
    for(int i=0; i<8; i++){
        std::cout << "+---+---+---+---+---+---+---+---+\n";
        for(int k=0; k<8; k++){
            std::cout << "| ";
            if ((k+i)%2 == 0){
                std::cout << "  ";
            }
            else{
                uint64_t mask = std::pow(2.0, 63-(8*i + k));
                if((mask & whiteKing) != 0){
                    std::cout << "\033[1;44;44mK\033[0m ";
                }
                else if((mask & blackKing) != 0){
                    std::cout << "\033[1;41;41mK\033[0m ";
                }
                else if ((mask & white) != 0){
                    std::cout << "\033[1;44;94m#\033[0m ";
                }
                else if((mask & black) != 0){
                    std::cout << "\033[1;41;91m#\033[0m ";
                }
                else{
                    std::cout << "  ";
                }
            }
        }
        std::cout << "|\n";
    }
    std::cout << "+---+---+---+---+---+---+---+---+\n\n" << std::flush;

}

std::vector<Move> Pos::genMoves(){
    std::vector<Move> moves;

    uint64_t c  = capture;
    uint64_t w  = white;
    uint64_t b  = black;
    uint64_t bk = blackKing;

    if(side == Side::white){
        w  = reverse(black);
        b  = reverse(white);
        bk = reverse(whiteKing);
        c  = reverse(capture);
    }

    uint64_t wb = ~(w | b) & legal_mask;
    uint64_t no_bking = b ^ bk;
    uint64_t wr7  = (w >> 7) & (wb >> 14) & n_first_2;
    uint64_t wr9  = (w >> 9) & (wb >> 18) & n_first_2;
    uint64_t wl7  = (w << 7) & (wb << 14) & r_first_2;
    uint64_t wl9  = (w << 9) & (wb << 18) & r_first_2;

    uint64_t captur_1 = c & no_bking & wr7;
    uint64_t captur_2 = c & no_bking & wr9;
    uint64_t king_c1  = c & bk       & wr7;
    uint64_t king_c2  = c & bk       & wr9;
    uint64_t king_c3  = c & bk       & wl7;
    uint64_t king_c4  = c & bk       & wl9;

    uint64_t captures = captur_1 | captur_2 | king_c1 | king_c2 | king_c3 | king_c4;

    if(captures != 0b0){
        if(c != legal_mask){
            if((c & bk) != 0){
                if(king_c1 != 0)
                    moves.push_back(Move(mType::capture,   c, c << 7, c << 14));
                if(king_c2 != 0)
                    moves.push_back(Move(mType::capture,   c, c << 9, c << 18));
                if(king_c3 != 0)
                    moves.push_back(Move(mType::capture,   c, c >> 7, c >> 14));
                if(king_c4 != 0)
                    moves.push_back(Move(mType::capture,   c, c >> 9, c >> 18));
            }
            else{
                if((captur_1 & n_first_3) != 0)
                    moves.push_back(Move(mType::capture,   c, c << 7, c << 14));
                if((captur_2 & n_first_3) != 0)
                    moves.push_back(Move(mType::capture,   c, c << 9, c << 18));
                if((captur_1 &   first_3) != 0)
                    moves.push_back(Move(mType::promotion, c, c << 7, c << 14));
                if((captur_2 &   first_3) != 0)
                    moves.push_back(Move(mType::promotion, c, c << 9, c << 18));
            }
            return moves;
        }
        for(uint64_t s: squares){
            if((captures & s) == 0) continue;
            if((captur_1 & n_first_3 & s) != 0)
                moves.push_back(Move(mType::capture,   s, s << 7, s << 14));
            if((captur_2 & n_first_3 & s) != 0)
                moves.push_back(Move(mType::capture,   s, s << 9, s << 18));
            if((captur_1 &   first_3 & s) != 0)
                moves.push_back(Move(mType::promotion, s, s << 7, s << 14));
            if((captur_2 &   first_3 & s) != 0)
                moves.push_back(Move(mType::promotion, s, s << 9, s << 18));
            if((king_c1 & s) != 0)
                moves.push_back(Move(mType::capture,   s, s << 7, s << 14));
            if((king_c2 & s) != 0)
                moves.push_back(Move(mType::capture,   s, s << 9, s << 18));
            if((king_c3 & s) != 0)
                moves.push_back(Move(mType::capture,   s, s >> 7, s >> 14));
            if((king_c4 & s) != 0)
                moves.push_back(Move(mType::capture,   s, s >> 9, s >> 18));
        }
        return moves;
    }
    else{
        uint64_t silent_1 = c & no_bking & (wb >> 7) &   first_1;
        uint64_t silent_2 = c & no_bking & (wb >> 9) &   first_1;
        uint64_t king_s1  = c & bk       & (wb >> 7) &   first_1;
        uint64_t king_s2  = c & bk       & (wb >> 9) &   first_1;
        uint64_t king_s3  = c & bk       & (wb << 7) & r_first_1;
        uint64_t king_s4  = c & bk       & (wb << 9) & r_first_1;

        uint64_t silents = silent_1 | silent_2 | king_s1 | king_s2 | king_s3 | king_s4;

        for(uint64_t s: squares){
            if((silents & s) == 0) continue;
            if((silent_1 & n_first_2 & s) != 0)
                moves.push_back(Move(mType::silent,    s, s << 7));
            if((silent_2 & n_first_2 & s) != 0)
                moves.push_back(Move(mType::silent,    s, s << 9));
            if((silent_1 &   first_2 & s) != 0)
                moves.push_back(Move(mType::promotion, s, s << 7));
            if((silent_2 &   first_2 & s) != 0)
                moves.push_back(Move(mType::promotion, s, s << 9));
            if((king_s1 & s) != 0)
                moves.push_back(Move(mType::silent,    s, s << 7));
            if((king_s2 & s) != 0)
                moves.push_back(Move(mType::silent,    s, s << 9));
            if((king_s3 & s) != 0)
                moves.push_back(Move(mType::silent,    s, s >> 7));
            if((king_s4 & s) != 0)
                moves.push_back(Move(mType::silent,    s, s >> 9));
        }
        return moves;
    }
}

void Pos::switch_side(){
    side = (side == Side::white) ? Side::black : Side::white;
}

void Pos::flip_board(){
    if(side == Side::white){
        uint64_t b = black;
        uint64_t w = white;
        uint64_t bk = blackKing;
        uint64_t wk = whiteKing;
        uint64_t cc = capture;
        uint8_t b_ = b_n;
        uint8_t w_ = w_n;
        uint8_t bK_ = bK_n;
        uint8_t wK_ = wK_n;
        black     = reverse(w);
        white     = reverse(b);
        blackKing = reverse(wk);
        whiteKing = reverse(bk);
        capture   = reverse(cc);
        b_n = w_;
        w_n = b_;
        bK_n = wK_;
        wK_n = bK_;
    }
}

void Pos::playMove(Move m){

    flip_board();

    if(m.type == mType::silent){
        black     |=  m.toSquare;
        black     &= ~m.fromSquare;
        if((blackKing & m.fromSquare) != 0){
            fifty++;
            blackKing |=  m.toSquare;
            blackKing &= ~m.fromSquare;
        }
        else{
            fifty = 0;
        }
        capture = legal_mask;
        flip_board();
        switch_side();
    }
    else if(m.type == mType::capture){
        fifty = 0;
        if((white & m.captureSquare) > 0){
            --w_n;
            if((whiteKing & m.captureSquare) >0) --wK_n;
        }

        black     |=  m.toSquare;
        black     &= ~m.fromSquare;
        white     &= ~m.captureSquare;
        whiteKing &= ~m.captureSquare;
        uint64_t wb = ~(white | black) & legal_mask;
        if((blackKing & m.fromSquare) != 0b0){
            blackKing |=  m.toSquare;
            blackKing &= ~m.fromSquare;
            uint64_t king_c1  = m.toSquare & blackKing & (white >> 7) & (wb >> 14) & n_first_2;
            uint64_t king_c2  = m.toSquare & blackKing & (white >> 9) & (wb >> 18) & n_first_2;
            uint64_t king_c3  = m.toSquare & blackKing & (white << 7) & (wb << 14) & r_first_2;
            uint64_t king_c4  = m.toSquare & blackKing & (white << 9) & (wb << 18) & r_first_2;
            if((king_c1 | king_c2 | king_c3 | king_c4) != 0b0){
                capture = m.toSquare;
                flip_board();
            }
            else{
                capture = legal_mask;
                flip_board();
                switch_side();
            }
        }
        else{
            uint64_t cap_1 = m.toSquare & (black ^ blackKing) & (white >> 7) & (wb >> 14) & n_first_2;
            uint64_t cap_2 = m.toSquare & (black ^ blackKing) & (white >> 9) & (wb >> 18) & n_first_2;
            if((cap_1 | cap_2) != 0b0){
                capture = m.toSquare;
                flip_board();
            }
            else{
                capture = legal_mask;
                flip_board();
                switch_side();
            }
        }
    }
    else if(m.type == mType::promotion){
        fifty = 0;
        if((white & m.captureSquare) > 0){
            --w_n;
            if((whiteKing & m.captureSquare) >0) --wK_n;
        }
        ++bK_n;

        blackKing |=  m.toSquare;
        black     |=  m.toSquare;
        black     &= ~m.fromSquare;
        white     &= ~m.captureSquare;
        whiteKing &= ~m.captureSquare;
        capture = legal_mask;
        flip_board();
        switch_side();
    }
    else{
        std::cout << "you discovered new type of a move, you fucking piece of shit\n";
    }

}

void Pos::playString(const std::string ft){
    std::string fS = {ft[0],ft[1]};
    std::string tS = {ft[2],ft[3]};
    if(side == Side::white){
        fS = i2s[reverse(s2i[fS])];
        tS = i2s[reverse(s2i[tS])];
    }

    std::vector<Move> moves = this->genMoves();
    for(Move m: moves){
        if((s2i[fS] == m.fromSquare) && (s2i[tS] == m.toSquare)){
            this->playMove(m);
            return;
        }
    }
}

Side Pos::getSide(){
    return side;
}

std::vector<uint8_t> Pos::serialize(){
    std::vector<uint8_t> ser;
    for(auto a: squares){
        ser.push_back(((black & a) > 0) ? 1 : 0);
    }
    for(auto a: squares){
        ser.push_back(((blackKing & a) > 0) ? 1 : 0);
    }
    for(auto a: squares){
        ser.push_back(((white & a) > 0) ? 1 : 0);
    }
    for(auto a: squares){
        ser.push_back(((whiteKing & a) > 0) ? 1 : 0);
    }
    return ser;
}

torch::Tensor Pos::getTensor(){
    torch::Tensor t = torch::zeros({4,8,4});
    for(uint8_t x=0; x<8; x++)
        for(uint8_t y=0; y<4; y++)
            if((black &     squares[4*x+y]) > 0) t[0][x][y] = 1;
    for(uint8_t x=0; x<8; x++)
        for(uint8_t y=0; y<4; y++)
            if((blackKing & squares[4*x+y]) > 0) t[1][x][y] = 1;
    for(uint8_t x=0; x<8; x++)
        for(uint8_t y=0; y<4; y++)
            if((white &     squares[4*x+y]) > 0) t[2][x][y] = 1;
    for(uint8_t x=0; x<8; x++)
        for(uint8_t y=0; y<4; y++)
            if((whiteKing & squares[4*x+y]) > 0) t[3][x][y] = 1;
    return t;
}

float Pos::one_v_one(){
    if(side == Side::white){
        if((whiteKing & b_mak) > 0){
            if((blackKing & b_mak) > 0) return -this->proximity();
            else                        return  this->proximity();
        }
        else{
            if((blackKing & b_mak) > 0) return  this->proximity();
            else                        return -this->proximity();
        }
    }
    else{
        if((blackKing & b_mak) > 0){
            if((whiteKing & b_mak) > 0) return  this->proximity();
            else                        return -this->proximity();
        }
        else{ 
            if((whiteKing & b_mak) > 0) return -this->proximity();
            else                        return  this->proximity();
        }
    }
}

bool Pos::is_one_v_one(){
    return (b_n == 1) && (bK_n == 1) && (w_n == 1) && (wK_n == 1);
}

float Pos::proximity(){
    return 0.5 + 0.5/std::max(abs(i2s[blackKing][0] - i2s[whiteKing][0]), 
                              abs(i2s[blackKing][1] - i2s[whiteKing][1]));
}

uint8_t Pos::getFifty(){
    return fifty;
}
