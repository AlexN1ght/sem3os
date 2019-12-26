#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <zmq.hpp>
#include <string>
#include <vector>
#include <unistd.h>
#include <iostream>
#include "Nodes.hpp"

class message {
    public: 
        enum { 
            ERR, CREATE,
            REMOVE, PING, REPLY, DEF,
            TAKEPORT, REPAR, EXEC, START,
            STOP, TIME, TERM, KILLNPASS
        };

        int type;
        int id;
        int data;
        
        message() {}
        message(Node& from) {
            recv(from);
        }
        message(int type, int id, int data) : type(type), id(id), data(data) {}
        void send(Node& to) {
            //puts("a");
            zmq::message_t typeMes(&type, sizeof(int));
            zmq::message_t idMes(&id, sizeof(int));
            zmq::message_t dataMes(&data, sizeof(int));
            //puts("b");
            to.Sock().send(typeMes, zmq::send_flags::sndmore);
            to.Sock().send(idMes, zmq::send_flags::sndmore);
            to.Sock().send(dataMes, zmq::send_flags::none);
            //puts("c");
        }
        void sendDW(Node& to) {
            //puts("a");
            zmq::message_t typeMes(&type, sizeof(int));
            zmq::message_t idMes(&id, sizeof(int));
            zmq::message_t dataMes(&data, sizeof(int));
            //puts("b");
            to.Sock().send(typeMes, zmq::send_flags::sndmore | zmq::send_flags::dontwait);
            to.Sock().send(idMes, zmq::send_flags::sndmore | zmq::send_flags::dontwait);
            to.Sock().send(dataMes, zmq::send_flags::sndmore | zmq::send_flags::dontwait);
            //puts("c");
        }
        void recv(Node& from) {
            //puts("aa");
            zmq::message_t typeMes;
            zmq::message_t idMes;
            zmq::message_t dataMes;
            from.Sock().recv(typeMes, zmq::recv_flags::none);
            from.Sock().recv(idMes, zmq::recv_flags::none);
            from.Sock().recv(dataMes, zmq::recv_flags::none);
            type = *((int*)(typeMes.data()));
            id = *((int*)(idMes.data()));
            data = *((int*)(dataMes.data()));
            //puts("cc");
        }
        
};

#endif
