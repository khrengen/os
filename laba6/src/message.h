#ifndef MESSAGE_H
#define MESSAGE_H

#include <zmq.hpp>
#include <string>
#include <ctime>
#include <unistd.h>
#include <iostream>
#include "node.hpp"

class message {
    public: 
        enum { 
            CREATE,REMOVE, PING, EXECFIND,
            EXECADD, KILL
        };

        int type;
        int id;
        int data;
        std::string name;
        
        message() 
        {
        }

        message(Node& mes) {
            recv(mes);
        }
        message(int type, int id, int data, std::string name) : type(type), id(id), data(data), name(name)
        {   
        }

        void send(Node& to) {
            zmq::message_t typeMes(&type, sizeof(int));
            zmq::message_t idMes(&id, sizeof(int));
            zmq::message_t dataMes(&data, sizeof(int));
            zmq::message_t nameMes(name.c_str(), sizeof(name.size() + 1));
            to.Sock().send(typeMes, zmq::send_flags::sndmore);
            to.Sock().send(idMes, zmq::send_flags::sndmore);
            to.Sock().send(dataMes, zmq::send_flags::sndmore);
            to.Sock().send(nameMes, zmq::send_flags::none);
        }
        void recv(Node& mes) {
            zmq::message_t typeMes;
            zmq::message_t idMes;
            zmq::message_t dataMes;
            zmq::message_t nameMes;

            mes.Sock().recv(typeMes, zmq::recv_flags::none);
            mes.Sock().recv(idMes, zmq::recv_flags::none);
            mes.Sock().recv(dataMes, zmq::recv_flags::none);
            mes.Sock().recv(nameMes, zmq::recv_flags::none);
            type = *((int*)(typeMes.data()));
            id = *((int*)(idMes.data()));
            data = *((int*)(dataMes.data()));
            char* char_name = (char*)(nameMes.data());
            name = (std::string)char_name;
        }

        int recvCheck(Node& mes) {
            zmq::message_t typeMes;
            zmq::message_t idMes;
            zmq::message_t dataMes;
            zmq::message_t nameMes;
            auto start = clock();
            while (true) { 
                if (mes.Sock().recv(typeMes, zmq::recv_flags::dontwait) &&
                    mes.Sock().recv(idMes, zmq::recv_flags::dontwait) &&
                    mes.Sock().recv(dataMes, zmq::recv_flags::dontwait) &&
                    mes.Sock().recv(nameMes, zmq::recv_flags::dontwait) )
                {
                    type = *((int*)(typeMes.data()));
                    id = *((int*)(idMes.data()));
                    data = *((int*)(dataMes.data()));
                    char* char_name = (char*)(nameMes.data());
                    name = (std::string)char_name;
                    return 1;
                } 
                if (clock() - start > (CLOCKS_PER_SEC))
                    return 0;
            }
        }

        
};

#endif