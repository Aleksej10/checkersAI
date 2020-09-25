#ifndef NODE_H
#define NODE_H

#include "pos.hpp"
#include "model.hpp"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <map>
#include <torch/torch.h>
#include <queue>
#include <future>


class Node {
private:
    Node * father_;
    Pos pos_;
    std::vector<Move>* moves_;
    uint8_t move_n_;
    uint8_t arg_max_;
    float estimate_;
    float score_; 
    bool leaf_;
    bool truly_;
    bool visited_; //TODO: obosolete? used only for printing
    std::vector<Node *> sons_;
public:
   ~Node();
    Node();
    Node(Node *, Pos, Move);
    static void delete_tree(Node *&);

    void show();
    void showTree();

    int8_t get_side(); //white 1, black -1
    float  get_score();
    float  get_estimate();
    bool   get_truly();
    bool   get_leaf();
    bool   get_visited();
    Node * get_father();
    Node * get_son(uint8_t);
    Pos    get_pos();
    std::vector<Move> * get_moves();
    uint8_t get_move_n();

    bool over();
    bool treefold();
    float result();
    uint8_t argmax();
    void expand();
    void backprop(bool, float);
    void trutify();
    int8_t monte();
    void monte(unsigned);

    static void playMove(Node *&, Move);
    static Move pick_n_play(Node *&, unsigned);

    static void self_play(Node *&, unsigned);
    static void self_vs_random(Node *&, unsigned, int);

    static std::pair<torch::Tensor, torch::Tensor> get_training_set(Node *&);
    static void install_net(Net);
    inline static Net net_;
};

#endif
