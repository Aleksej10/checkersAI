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

typedef websocketpp::client<websocketpp::config::asio_client> client;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) result.push_back(item);
    return result;
}

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

    void playMove(std::string m){
        if(node_->get_side() != playerSide_){//TODO: premove?
            std::cout << "ai " << id_ << " " << Node::pick_n_play(node_, lvl_) << '\n';
            return;
        }
        else{
            Node::playMove(node_, m); 
            if(node_->get_side() == playerSide_){
                return;
            }
            else{
                while(node_->get_side() != playerSide_){
                    std::cout << "ai " << id_ << " " << Node::pick_n_play(node_, lvl_) << '\n';
                }
                return;
            }
        }
    }

    void show(){ node_->show(); }
};


class Server{
private:
    std::map<std::string, Game*> games_;
public:
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

    void addGame(std::string id, int s, unsigned lvl){
        games_[id] = new Game(id, s, lvl);
        if(s == 1){
            auto fut = std::async(std::launch::async, [this, id]{return games_[id]->playMove("");});
            fut.get();
        }
    }

    void start(){
        while(true){
            std::string msg; //TODO get from server
            std::getline(std::cin, msg);
            std::vector<std::string> v = split(msg, ' ');
            std::string gameID = v[1];

            if(v[0] == "game"){//['game', gameID, side, level]
                int s = std::stoi(v[2]);
                unsigned lvl = std::stoi(v[3]);
                this->addGame(gameID, s, lvl);
            }
            else if(v[0] == "move"){ //['move', gameID, move, time]
                std::string mv = v[2];
                unsigned millis = std::stoi(v[3]);
                auto t0 = std::chrono::high_resolution_clock::now();
                auto fut = std::async(std::launch::async, [this, mv, gameID]{return games_[gameID]->playMove(mv);});
                auto t1 = std::chrono::high_resolution_clock::now();
                auto t = std::chrono::duration_cast<std::chrono::milliseconds>(t1-t0).count() + millis;
                fut.get();
            }
            else if(v[0] == "end"){
                //TODO obsolete
            }
            else if(v[0] == "show"){
                games_[gameID]->show();
            }
            else{
                std::cout << "unknown message: " << v[0] << '\n';
            }
        }
    }
};

Server server;

void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg) {
    std::vector<std::string> v = split(msg->get_payload(), ' ');
    for(auto arg: v){
        std::cout << arg << '\n';
    }

    websocketpp::lib::error_code ec;

    c->send(hdl, "I is alive", msg->get_opcode(), ec);
    if (ec) {
        std::cout << "Error sending message: " << ec.message() << std::endl;
    }
}


int main(int argc, char *argv[]) {
    /* client c; */

    /* std::string uri = "ws://localhost:8081"; */

    /* try { */
    /*     c.clear_access_channels(websocketpp::log::alevel::all); */

    /*     c.init_asio(); */

    /*     c.set_message_handler(bind(&on_message,&c,::_1,::_2)); */

    /*     websocketpp::lib::error_code ec; */
    /*     client::connection_ptr con = c.get_connection(uri, ec); */
    /*     if (ec) { */
    /*         std::cout << "could not create connection because: " << ec.message() << std::endl; */
    /*         return 0; */
    /*     } */

    /*     c.connect(con); */

    /*     c.run(); */
    /* } catch (websocketpp::exception const & e) { */
    /*     std::cout << e.what() << std::endl; */
    /* } */
    server.start();

    return 0;
}


