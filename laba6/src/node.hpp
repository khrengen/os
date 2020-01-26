#ifndef NODES_HPP
#define NODES_HPP

#include <zmq.hpp>
#include <string>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <map>


int BEGIN_PORT = 30000;
const char* BIND_URL = "tcp://*:";
const char* CON_URL = "tcp://localhost:";

std::string BindURLPort(int port) {
	return BIND_URL + std::to_string(port);
}

std::string ConURLPort(int port) {
	return CON_URL + std::to_string(port);
}

int TakePort(zmq::socket_t& socket)
{
	int port = BEGIN_PORT;
	while (true) {
		try {
			socket.bind(BindURLPort(port));
			break;
		} catch (const zmq::error_t& err) {
			++port;
		}
	}
	return port;
}

class Node {
    public:
        enum { 
            PARENT, NEW
        };
        Node() : id(-2),  ctx(1),socket(ctx, ZMQ_PAIR),  number(0)
        {	
        }
        Node(int type, int id, int number, int inPort = -1) : ctx(1), socket(ctx, ZMQ_PAIR), id(id), number(number) {
             switch(type) {
                case PARENT:
                    port = inPort;
                    socket.connect(ConURLPort(port));
                    break;
                case NEW:
                    port = TakePort(socket);
                    pid = fork();
                    if (pid == 0) {
                        using std::to_string;
                        execl("WNode", to_string(id).c_str(), to_string(number).c_str(), to_string(port).c_str(), (char*)NULL);
                    }
                    break;
            }
        }
        void NewNode(int inId, int num) {
            id = inId;
            number = num;
            port = TakePort(socket);
            pid = fork();
            if (pid == 0) {
                using std::to_string;
                execl("WNode", to_string(id).c_str(), to_string(number).c_str(), to_string(port).c_str(), (char*)NULL);
            }
        }

        pid_t Pid() {
            return pid;
        }
        int Id() {
            return id;
        }
        int Port() {
            return port;
        }
        zmq::socket_t& Sock() {
            return socket;
        }

    private:
        int id;
        int port;
        pid_t pid;
        int number;
        zmq::context_t ctx;
        zmq::socket_t socket;
};

#endif