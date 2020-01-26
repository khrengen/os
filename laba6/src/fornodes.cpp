#include <zmq.hpp>
#include <string>
#include <vector>
#include <chrono>
#include "node.hpp"
#include "message.h"
#include <vector>


void rec(int x, int res, std::vector<int> &v) {
    if (x == res) {
        v.push_back(x);
        return;
    }
    while (x < res) {
        x *= 2;
        v.push_back(x);
        rec(x, res, v);
        x++;
        v.push_back(x);
        rec(x,res, v);
    } 
}

int reccheck(int x, int res) {
    std::vector<int> v;
    rec(x,res,v);
    for (int n : v) {
        if (n == res) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char** argv) {
	if (argc != 3) {
		return 0;
    }

    int work = 0;
    int Id = atoi(argv[0]);
    int number = atoi(argv[1]);
    int left = number * 2;
    int right = number * 2 + 1;
	Node* Parent = new Node(Node::PARENT, 0, 0, atoi(argv[2]));
    Node* leftChild = new Node;
    Node* rightChild = new Node;
    message to;
    message mes;
    std::map<std::string, int> kv;

    int check;    
    
    while(true) {
        mes.recv(*Parent);
        if (work == 0) {
        switch (mes.type) {
            case message::CREATE:
                if (Id == mes.data) {
                    if (leftChild->Id() == -2) {
                        leftChild->NewNode(mes.id, left);
                        mes.data = leftChild->Pid();
                        mes.send(*Parent);
                        
                    } else if (rightChild->Id() == -2) {
                        rightChild->NewNode(mes.id, right);
                        mes.data = rightChild->Pid();
                        mes.send(*Parent);
                    }
                    
                } else {
                    // drugim kidaem

                    if (reccheck(left, mes.id)) {
                        mes.send(*leftChild);
                        if(to.recvCheck(*leftChild)) {
                            to.send(*Parent);
                        }
                    } else {
                        mes.send(*rightChild); 
                        if(to.recvCheck(*rightChild)) {
                            to.send(*Parent);
                        }              
                    }

                  
                }
                break;
           

            case message::REMOVE:

                if (Id == mes.id) {
                    work = 1;
                    mes.send(*Parent);

                   
                } else {

                     if (reccheck(left, mes.id)) {
                        mes.send(*leftChild);
                        if(to.recvCheck(*leftChild)) {
                            to.send(*Parent);
                        }
                    } else {
                        mes.send(*rightChild); 
                        if(to.recvCheck(*rightChild)) {
                            to.send(*Parent);
                        }              
                    }
                }

                break;


            case message::EXECFIND:
                if (Id == mes.id) {
                    mes.data = -1;
                    for (auto it = kv.begin(); it != kv.end(); ++it) {
                        if (it->first == mes.name) {
                            mes.data = it->second;
                            break;
                        }
                    }
                    mes.send(*Parent);

                } else {
                     if (reccheck(left, mes.id)) {
                        mes.send(*leftChild);
                        if(to.recvCheck(*leftChild)) {
                            to.send(*Parent);    
                        }
                    } else {
                        mes.send(*rightChild); 
                        if(to.recvCheck(*rightChild)) {
                            to.send(*Parent);
                        }              
                    }
                }
                break;

            case message::EXECADD:
                check = 0;
                if (Id == mes.id) {
                    for (auto it = kv.begin(); it != kv.end(); ++it) {
                        if (it->first == mes.name) {
                            //std::cout << it->first << "; " << mes.name << '\n';
                            check = 1;

                        }
                    }
                    
                    if (check == 0) {
                        kv.insert(std::pair<std::string,int>(mes.name,mes.data));
                        
                    } else {
                        mes.data = -1;
                    }
                    mes.send(*Parent);

                } else {
                     if (reccheck(left, mes.id)) {
                        mes.send(*leftChild);
                        if(to.recvCheck(*leftChild)) {
                            to.send(*Parent);
                        }
                    } else {
                        mes.send(*rightChild); 
                        if(to.recvCheck(*rightChild)) {
                            to.send(*Parent);
                        }              
                    }
                }
                break;

            case message::PING:
                if (Id == mes.id) {
                    mes.data = Id;
                    mes.send(*Parent);

                } else {
                    if (reccheck(left, mes.id)) {
                        mes.send(*leftChild);
                        if(to.recvCheck(*leftChild)) {
                            to.send(*Parent);
                        }
                    } else {
                        mes.send(*rightChild); 
                        if(to.recvCheck(*rightChild)) {
                            to.send(*Parent);
                        }              
                    }
                }
                
                break;

              case message::KILL:
                printf("i will kill myself\n");
                if (leftChild->Id() != -2) {
                    mes.send(*leftChild);
                }
                if (rightChild->Id() != -2) {
                    mes.send(*rightChild);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                return 0;
                break;
        }
        }
    }
	return 0;
}