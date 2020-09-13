#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>
#include "pos.hpp"
#include "node.hpp"
#include "model.hpp"
#include <iostream>
#include <map>
#include <string>
#include <sstream>
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


/* std::vector<std::string> split(const std::string &s, char delim) { */
/*     std::vector<std::string> result; */
/*     std::stringstream ss(s); */
/*     std::string item; */
/*     while (getline(ss, item, delim)) result.push_back(item); */
/*     return result; */
/* } */

json move_to_json(Move m){
    const json j = {{"_captureSquare" , m.captureSquare},
                    {"_fromSquare", m.fromSquare},
                    {"_toSquare", m.toSquare},
                    {"_type", m.type} };
    return j;
}

Move json_to_move(json j){
    return Move(j["_type"].get<mType>(),
                j["_fromSquare"].get<uint64_t>(),
                j["_toSquare"].get<uint64_t>(),
                j["_captureSquare"].get<uint64_t>());
}

std::string userID;

class Game{
private:
    std::string id_; 
    int playerSide_; 
    unsigned lvl_;
    Node* node_;

public:
    ~Game(){}
    Game(std::string id, int s, unsigned lvl): id_(id), playerSide_(s), lvl_(lvl), node_(new Node()){}

    std::string get_id(){ return id_; }
    int get_pside(){ return playerSide_; }
    unsigned get_lvl(){ return lvl_; }
    Node * get_node(){ return node_; }

    std::vector<Move> playMove(Move m){
        if(node_->get_side() != playerSide_){//TODO: premove?
            return std::vector<Move> {Node::pick_n_play(node_, lvl_)};
            /* const json j = {"move", id_, userID, Node::pick_n_play(node_, lvl_)}; */
            /* /1* std::cout << "ai " << id_ << " " << Node::pick_n_play(node_, lvl_) << '\n'; *1/ */
            /* return ; */
        }
        else{
            Node::playMove(node_, m); 
            if(node_->get_side() == playerSide_){
                return {};
            }
            else{
                std::vector<Move> moves;
                while(node_->get_side() != playerSide_){
                    moves.push_back(Node::pick_n_play(node_, lvl_));
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

    ~Server(){
        for(auto [_, g]: games_){
            delete(g);
        }
    }
    Server(){
        Model m = Model();
        m.load();
        Node::install_net(m.get_net());
    }

    std::vector<Move> addGame(std::string id, int s, unsigned lvl){
        games_[id] = new Game(id, s, lvl);
        if(s == 1){
            Move token;
            return games_[id]->playMove(token);
            /* auto fut = std::async(std::launch::async, [this, id]{return games_[id]->playMove("");}); */
            /* return fut.get(); */
        }
        else{
            return {};
        }
    }

    /* void start(){ */
    /*     while(true){ */
    /*         else if(v[0] == "move"){ //['move', gameID, move, time] */
    /*             std::string mv = v[2]; */
    /*             unsigned millis = std::stoi(v[3]); */
    /*             auto t0 = std::chrono::high_resolution_clock::now(); */
    /*             auto fut = std::async(std::launch::async, [this, mv, gameID]{return games_[gameID]->playMove(mv);}); */
    /*             auto t1 = std::chrono::high_resolution_clock::now(); */
    /*             auto t = std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count() + millis; */
    /*             auto ret = fut.get(); */
    /*         } */
    /*         else if(v[0] == "end"){ */
    /*             //TODO obsolete */
    /*         } */
    /*         else if(v[0] == "show"){ */
    /*             games_[gameID]->show(); */
    /*         } */
    /* } */
};

Server server;

void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr message) {
    json msg = json::parse(message->get_payload());

    websocketpp::lib::error_code ec;

    if(msg[0].get<std::string>() == "handshake"){
        userID = msg[1].get<std::string>();
        json ret = {"signin", userID, "ai", "e2e5a999af141629ba7fdd8d6dd50d59475d20ebbd763c70c39eecc648f4d1e1"};
        c->send(hdl, ret.dump(), message->get_opcode(), ec);
    }
    else if(msg[0].get<std::string>() == "signed"){
        if(msg[1].get<std::string>() != "success"){
            std::cout << "couldn't sign in" << '\n';
            exit(EXIT_FAILURE);
        }
    }
    else if(msg[0].get<std::string>() == "game"){
        std::string gameID = msg[1].get<std::string>();
        int side = msg[2].get<int>();
        unsigned level = msg[3].get<unsigned>();
        auto fut = std::async(std::launch::async, [gameID, side, level]{return server.addGame(gameID, side, level);});
        auto moves = fut.get();
        if(moves.size() == 1){
            json ret = {"move", gameID, userID, move_to_json(moves[0]), 3 * 60 * 100};
            c->send(hdl, ret.dump(), message->get_opcode(), ec);
        }
    }
    else if(msg[0].get<std::string>() == "move"){
        std::string gameID = msg[1].get<std::string>();
        // msg[2] opponent id, obsolete
        auto mv = msg[3]; //TODO unpack move
        Move move = json_to_move(mv);
        unsigned millis = msg[4].get<unsigned>();

        auto t0 = std::chrono::high_resolution_clock::now();
        auto fut = std::async(std::launch::async, [move, gameID]{return server.games_[gameID]->playMove(move);});
        auto t1 = std::chrono::high_resolution_clock::now();
        auto t = std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count() + millis;
        auto moves = fut.get();
        for(auto m: moves){
            json ret = {"move", gameID, userID, move_to_json(m), t};
            c->send(hdl, ret.dump(), message->get_opcode(), ec);
        }
    }
    else{
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

    /* server.start(); */

    return 0;
}


