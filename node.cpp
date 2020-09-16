#include "node.hpp"

Node::~Node(){ // deletes nodes bellow it
    for (auto s : sons_)
        delete s;
    delete father_;
}

void Node::delete_tree(Node *& leaf){ // starting from any node, finds root and deletes the tree
    Node * tmp = leaf;
    while(tmp->get_father() != nullptr)
        tmp = tmp->get_father();
    delete tmp;
}

Node::Node(){
    father_ = nullptr;
    pos_ = Pos();
    moves_ = pos_.genMoves();
    move_n_ = moves_.size();
    estimate_ = Node::net_->forward(pos_.getTensor().unsqueeze(0))[0].item<float>();
    score_ = estimate_;
    leaf_ = true;
    truly_ = false;
    visited_ = false;
}

Node::Node(Node * father, Pos pos, Move m){
    father_ = father;
    pos_ = pos; pos_.playMove(m);
    moves_ = pos_.genMoves();
    move_n_ = moves_.size();
    estimate_ = Node::net_->forward(pos_.getTensor().unsqueeze(0))[0].item<float>();
    score_ = estimate_;
    leaf_ = true;
    truly_ = false;
    visited_ = false;
}

// Visuals
void Node::show(){
    pos_.prettyPrint();
    std::cout << "\nSide to move: " << pos_.side << "\n";
    std::cout << "Initial estimate: " << estimate_ << "\n";
    std::cout << "leaf?: " << leaf_ << "\n";
    if(truly_) std::cout << "Calculated ";
    else       std::cout << "Estimated ";
    std::cout << "value: " << score_ << "\n";
    std::cout << "moves(" << move_n_ << "):\n";
    if(!leaf_){
        for(uint8_t i=0; i<move_n_; i++){
            if (sons_[i]->get_visited()) std::cout << "check ";
            else                         std::cout << "      ";
        }
        std::cout << '\n';
    }
    for(auto m: moves_)
        std::cout << m.toString() << " ";
    std::cout << '\n';
}

void Node::showTree(){
    this->show();
    if(sons_.empty()) return;
    for (auto s : sons_) {
        if(!s->get_visited()) continue;
        s->showTree();
    }
}

// Getters
int Node::get_side(){ return pos_.side; }

float Node::get_score(){ return score_; }

float Node::get_estimate(){ return estimate_; }

bool Node::get_truly(){ return truly_; }

bool Node::get_leaf(){ return leaf_; }

bool Node::get_visited(){ return visited_; }

Node * Node::get_father(){ return father_; }

Node * Node::get_son(unsigned i){ return sons_[i]; }

Pos Node::get_pos(){ return pos_; }

std::vector<Move>* Node::get_moves(){ return &moves_; }

unsigned Node::get_move_n(){ return move_n_; }

// Tree search
bool Node::treefold(){
    uint8_t k = 1;
    Node * tmp = this;
    while(tmp->get_father() != nullptr){
        if(tmp->get_pos() == pos_){
            k += 1;
            if(k == 3) return true;
        }
        tmp = tmp->get_father();
    }
    return false;
}

bool Node::over(){
    return ((move_n_ == 0) || (pos_.fifty > 25) || this->treefold()) ? true : false;
}

float Node::result(){
    return ((pos_.fifty > 25) || this->treefold()) ? 0 : -pos_.side;
}

uint8_t Node::argmax(){ return arg_max_; }

void Node::expand(){
    if(!leaf_) return;
    if(this->over()) return;
    for (auto m : moves_)
        sons_.push_back(new Node(this, pos_, m));
    leaf_ = false;
}

void Node::backprop(bool leaf, float son_score){
    visited_ = true;
    this->expand();

    if(this->over()){
        score_ = this->result();
        truly_ = true;
    }
    else{
        this->trutify();
    }

    if(!truly_ && leaf){
        score_ = (score_ + son_score)/2.0;
    }

    if(father_ != nullptr) father_->backprop(true, score_);
}

void Node::trutify(){ 
    float  max = sons_[0]->get_score();
    uint8_t maxi = 0;
    bool max_t = sons_[0]->get_truly();

    for(uint8_t i=1; i<move_n_; i++){
        float sc = sons_[i]->get_score();
        bool son_t = sons_[i]->get_truly();
        if(son_t && (pos_.side * sc > 0)){
            truly_ = true;
            score_ = sc;
            arg_max_ = i;
            return;
        }
        if((pos_.side * sc) > (pos_.side * max)){
            max = sc;
            maxi = i;
            max_t = son_t;
        }
    }
    truly_ = max_t;
    score_ = max;
    arg_max_ = maxi;
}

int Node::monte(){ // return -1 = done
    if(truly_) return -1;
    if(move_n_ == 1){
        this->backprop(false, 0 /*token*/);
        return -1;
    }
    Node * tmp = this;
    while(!tmp->get_leaf() && !tmp->over() && !tmp->get_truly()){
        tmp = tmp->get_son(tmp->argmax());
    }
    tmp->backprop(false, 0 /*token*/);
    return 1;
}

void Node::monte(unsigned n){
    for(unsigned i=0; i<n; i++){
        if(this->monte() < 0)return;
    }
}

// Interfaces
void Node::playMove(Node *& n, Move m){
    n->expand();

    std::vector<Move> mvs = *(n->get_moves());

    int index = -1;
    for(unsigned p=0; p<mvs.size(); p++){
        if(m == mvs[p]){
            index = p;
            break;
        }
    }
    if(index == -1){
        std::cout << "move " << m.toString() << " not found\n";
        return;
    }
    n = n->get_son(index);
}

Move Node::pick_n_play(Node *& n, unsigned lvl){
    n->monte(lvl);
    uint8_t index = n->argmax();
    std::vector<Move> mvs = *(n->get_moves());
    n = n->get_son(index);
    return mvs[index];
}


void Node::self_play(Node *& n, unsigned i){
    while(!n->over()){
        n->monte(i);
        /* n->show(); */
        n = n->get_son(n->argmax());
    }
}

void Node::self_vs_random(Node *& n, unsigned i, int s){
    while(!n->over()){
        if(n->get_side() == s){
            n->monte();
            /* n->show(); */
            n = n->get_son(rand() % n->get_move_n());
        }
        else{
            n->monte(i);
            /* n->show(); */
            n = n->get_son(n->argmax());
        }
    }
}

// Model
/* std::pair<torch::Tensor, torch::Tensor> Node::get_training_set(Node * n){ */ // ---top to bottom TODO: integrate with bottom-up
/*     std::queue<Node *> q; q.push(n); */
/*     std::vector<torch::Tensor> xs; */
/*     std::vector<torch::Tensor> ys; */

/*     while(!q.empty()){ */
/*         unsigned k = q.front()->get_move_n(); */
/*         for(unsigned i=0; i<k; i++){ */
/*             Node * tmp = q.front()->get_son(i); */
/*             if(tmp->get_visited()) q.push(tmp); */
/*         } */
/*         ys.push_back(torch::tensor({q.front()->get_score()})); */
/*         xs.push_back(q.front()->get_pos().getTensor()); */
/*         q.pop(); */
/*     } */
/*     return std::make_pair(torch::stack(xs), torch::stack(ys)); */
/* } */

std::pair<torch::Tensor, torch::Tensor> Node::get_training_set(Node *& n){
    int result = n->result();
    std::vector<torch::Tensor> xs;
    std::vector<torch::Tensor> ys;
    unsigned k = 0;
    while(n->get_father() != nullptr){
        float update;
        if(n->get_truly()){ update = n->get_score(); }
        else{               update = (result + n->get_score())/2.0; }
        ys.push_back(torch::tensor({update}));
        xs.push_back(n->get_pos().getTensor());
        n = n->get_father();
        k++;
    }
    std::cout << k << " moves picked for training\n";
    return std::make_pair(torch::stack(xs), torch::stack(ys));
}

void Node::install_net(Net n){ Node::net_ = n; }

float Node::play_n_train(Model * m, unsigned diff){
    Node * n = new Node();
    Node::self_play(n, diff);
    std::pair<torch::Tensor, torch::Tensor> p = Node::get_training_set(n);
    torch::Tensor xs = p.first;
    torch::Tensor ys = p.second;
    delete n;
    return m->train(xs, ys);
}
