#include <bits/stdint-uintn.h>
#include <cstdlib>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include "pos.hpp"
#include "node.hpp"
#include "model.hpp"
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <future>
#include <chrono>
#include "json.hpp"

typedef websocketpp::client<websocketpp::config::asio_client> client;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
using json = nlohmann::json;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;


json move_to_json(Move m){
    int type;
    mType t = m.type;

    if(t == mType::silent) type = 0;
    else if(t == mType::capture) type = 1;
    else if(t == mType::promotion) type = 2;
    else{
        std::cout << "type " << t << " not recognized\n";
        exit(EXIT_FAILURE);
    }
    return json {{"_captureSquare" , m.captureSquare},
                {"_fromSquare", m.fromSquare},
                {"_toSquare", m.toSquare},
                {"_type", type} };
}

Move json_to_move(json j){
    mType type;
    int t = j["_type"].get<int>();

    if(t == 0) type = mType::silent;
    else if(t == 1) type = mType::capture;
    else if(t == 2) type = mType::promotion;
    else{
        std::cout << "type " << t << " not recognized\n";
        exit(EXIT_FAILURE);
    }
    return Move(type,
                std::stoull(j["_fromSquare"].get<std::string>()),
                std::stoull(j["_captureSquare"].get<std::string>()),
                std::stoull(j["_toSquare"].get<std::string>()));

}

std::string userID;

class Game{
private:
    std::string id_; 
    int playerSide_; 
    unsigned lvl_;

public:
    Node* node_;
    ~Game(){ 
        delete node_;
    }

    Game(const std::string & id, int s, unsigned lvl): id_(id), playerSide_(s), lvl_(lvl), node_(new Node()){}
    int get_pSide(){ return playerSide_; }

    std::vector<Move> playMove(Move m){
        if(node_->get_side() != playerSide_){//TODO: premove? currently used to play first move for AI
            std::cout << "playing first move in game " << id_ << '\n';
            return std::vector<Move> { Node::pick_n_play(node_, lvl_) };
        }
        else{
            Node::playMove(node_, m); 
            std::cout << "played opponents move in game " << id_ << '\n';
            if(node_->get_side() == playerSide_){
                std::cout << "still opponents turn in game " << id_ << '\n';
                return std::vector<Move> {};
            }
            else{
                std::vector<Move> moves;
                while((node_->get_side() != playerSide_) && (!node_->over())){
                    moves.push_back(Node::pick_n_play(node_, lvl_));
                }
                std::cout << "played " << moves.size() << " move(s) in game " << id_ << '\n';
                if(node_->over()){
                    std::cout << "game " << id_ << " ended\n";
                }
                return moves;
            }
        }
    }

    void show(){ node_->show(); }
};


class Server{
public:
    std::map<std::string, Game*> games_;
    Model* m;

    ~Server(){
        for(auto [_, g]: games_)
            delete g;
    }

    Server(){
        m = new Model();
        m->load();
        Node::install_net(m->get_net());
    }

    std::vector<Move> addGame(const std::string & id, int s, unsigned lvl){
        std::cout << "game " << id << " added\n";
        games_[id] = new Game(id, s, lvl);
        if(s == 1){
            /* Move token; */
            return games_[id]->playMove(Move());
        }
        else{
            return std::vector<Move> {};
        }
    }
};

Server server;

void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr message) {
    json msg = json::parse(message->get_payload());
    std::string msg0 = msg[0].get<std::string>();

    websocketpp::lib::error_code ec;

    if(msg0 == "handshake"){  // ['handshake', userID], 
        userID = msg[1].get<std::string>();
        json ret = {"signin", userID, "ai", "e2e5a999af141629ba7fdd8d6dd50d59475d20ebbd763c70c39eecc648f4d1e1"};
        c->send(hdl, ret.dump(), message->get_opcode(), ec);
    }
    else if(msg0 == "signed"){ // ['signed', status, name, elo],
        if(msg[1].get<std::string>() != "success"){
            std::cout << "couldn't sign in" << '\n';
            exit(EXIT_FAILURE);
        }
    }
    else if(msg0 == "game"){ // ['game', gameID, side, level, start_time],
        std::string gameID = msg[1].get<std::string>();
        int side = msg[2].get<int>();
        unsigned level = msg[3].get<unsigned>();
        unsigned millis = msg[4].get<unsigned>();

        auto t0 = std::chrono::high_resolution_clock::now();
            auto fut = std::async(std::launch::async, [gameID, side, level]{return server.addGame(gameID, side, level);});
            std::vector<Move> moves = fut.get();
        auto t1 = std::chrono::high_resolution_clock::now();
        auto t = millis - std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count();

        if(moves.size() == 1){
            json ret = {"move", gameID, userID, move_to_json(moves[0]), t};
            c->send(hdl, ret.dump(), message->get_opcode(), ec);
        }
    }
    else if(msg0 == "move"){ // ['move', gameID, move, time]
        std::string gameID = msg[1].get<std::string>();
        Move move = json_to_move(msg[2]);
        unsigned millis = msg[3].get<unsigned>();

        auto t0 = std::chrono::high_resolution_clock::now();
            auto fut = std::async(std::launch::async, [move, gameID]{return server.games_[gameID]->playMove(move);});
            std::vector<Move> moves = fut.get();
        auto t1 = std::chrono::high_resolution_clock::now();
        auto t = millis - std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count();

        for(auto m: moves){
            json ret = {"move", gameID, userID, move_to_json(m), t};
            c->send(hdl, ret.dump(), message->get_opcode(), ec);
        }
    }
    else if(msg0 == "end"){ // ['end', gameID, score, rating_shift]
        std::string gameID = msg[1].get<std::string>();
        std::cout << "game " << gameID << " ended\n";

        std::pair<torch::Tensor, torch::Tensor> p = Node::get_training_set(server.games_[gameID]->node_);
        torch::Tensor xs = p.first;
        torch::Tensor ys = p.second;

        server.games_.erase(gameID);

        std::cout << "loss: " << server.m->train(xs, ys) << '\n';
        server.m->save();
    }
    else{
        std::cout << "unrecognized message:\n";
        std::cout << msg.dump(2) << '\n';
    }

    if(ec){ std::cout << "Error sending message: " << ec.message() << '\n'; }
}


int main(int argc, char *argv[]) {
    client c;

    std::string uri = "ws://localhost:8081";

    try {
        c.clear_access_channels(websocketpp::log::alevel::all);

        c.init_asio();

        c.set_message_handler(bind(&on_message,&c,::_1,::_2));

        websocketpp::lib::error_code ec;
        client::connection_ptr con = c.get_connection(uri, ec);
        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
            return 0;
        }

        c.connect(con);

        c.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
