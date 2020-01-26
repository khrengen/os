#include <iostream>
#include <zmq.hpp>
#include <string>
#include <queue>
#include <map>
#include "node.hpp"
#include "message.h"

enum { 
    ERR, CREATE,
    REMOVE, EXEC,
    PING, EXIT
};


int main(void) {

    int id, parent;
    std::queue<int> qu;
    std::map<int,int> allNodes;
    int count = 0;
    int exec = 0;
    //
    message to;
    message mes;

    //message parsing
    std::string cmd;
    std::string name;
    int value;
    Node* root;
    puts("Start");
    for (;;) {
        std::cin >> cmd;
        if (cmd == "Create") {
            //creating
            std::cin  >> id;
            char c = '0';
            while (c != '\n') {
                c = getchar();
            }
            if (allNodes.find(id) != allNodes.end()) {
                std::cout << "Error: Already exists\n";
            } else {
                if (qu.empty()) {
                    //its root
                    root = new Node(Node::NEW, id, 1, -1);
                    qu.push(id);
                    allNodes.insert(std::pair<int, int>(id, -1));
                    std:: cout << "OK: " << root->Pid() << '\n';
                } else {    
                    allNodes.insert(std::pair<int,int>(id,qu.front()));
                    to.type = message::CREATE;
                    to.id = id;
                    to.data = qu.front();//parent
                    to.send(*root);
                    if(!mes.recvCheck(*root)) {
                        std::cout << "create error\n";
                    } else {
                        std::cout << "OK: " << mes.data << '\n';
                        qu.push(id);
                        count++;
                        if (count == 2) {
                            qu.pop();
                            count = 0;
                        }
                    }
                    
                }
            }

        } else if (cmd == "Remove") {
            std::cin >> id;
            to.type = message::REMOVE;
            to.id = id;
            to.send(*root);
            if(!mes.recvCheck(*root)) {
                std::cout << "remove error\n";
            } else {
                std::cout << "OK: " << mes.id << '\n';
            } 

        } else if (cmd == "Exec") {
            char c;
            std::cin >> id;
            std::cin >> name;
            c = getchar();
            if (c == '\n') {
                exec = 1;
            } else {
                std::cin >> value;
            }
            if (exec == 1) {
                to.type = message::EXECFIND;
                to.id = id;
                to.name = name;
                to.send(*root);
                if(!mes.recvCheck(*root)) {
                    std::cout << "node is unanailable\n";
                } else {
                    if (mes.data != -1){
                        std::cout << "OK: " << mes.id << ": " << mes.data << '\n';
                    } else {
                        std::cout << name << " not found\n";
                    }
                }
                exec = 0;

                
            } else {
                to.type = message::EXECADD;
                to.data = value;
                to.id = id;
                to.name = name;
                to.send(*root);
                if (!mes.recvCheck(*root)) {
                    std::cout << "node is nedostupen\n";
                } else {
                    if (mes.data != -1) {
                        std::cout << "OK: " << mes.id << '\n';
                    } else {
                        std::cout << "Already exist\n";
                    }
                }
            }
            
        } else if (cmd == "Ping") {
            std::cin >> id;
            if (allNodes.find(id) == allNodes.end()) {
                std::cout << "Error: not found\n";
            } else {
                to.type = message::PING;
                to.id = id;
                to.send(*root);

                if(!mes.recvCheck(*root)) {
                std::cout << "OK: 0\n";
                } else {
                    std::cout << "OK: " << mes.data << '\n';
                }
            }
        } else if (cmd == "EXIT") {
            to.type = message::KILL;
            to.send(*root);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            delete root;
            std::cout << "Ending....\n";
            return 0;
        } else {
            std::cout << "Wrong Command\n";
            
        }
    }
    

   
}