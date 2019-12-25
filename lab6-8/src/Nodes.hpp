#ifndef NODES_HPP
#define NODES_HPP

#include <zmq.hpp>
#include <string>
#include <vector>
#include <unistd.h>
#include <iostream>
#include <thread>
//#include <map>

int BEGIN_PORT = 30000;
const char* BIND_URL = "tcp://*:";
const char* CON_URL = "tcp://localhost:";

std::string BindURLPort(int port) {
	return BIND_URL + std::to_string(port);
}

std::string ConURLPort(int port) {
	return CON_URL + std::to_string(port);
}

int TakePort(zmq::socket_t& sock)
{
	int port = BEGIN_PORT;
	while (true) {
		try {
			sock.bind(BindURLPort(port));
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
            ERR, CHILD,
            PARENT, NEW, ADD
        };
        Node() : id(-2), ctx(1), sock(ctx, ZMQ_PAIR) {}
        Node(int type, int id, int inPort = -1) : ctx(1), sock(ctx, ZMQ_PAIR), id(id) {
            switch(type) {
                case CHILD:
                    port = inPort;
                    sock.bind(BindURLPort(port));
                    break;
                case PARENT:
                    port = inPort;
                    sock.connect(ConURLPort(port));
                    //std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    break;
                case NEW:
                    port = TakePort(sock);
                    //std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    pid = fork();
                    if (pid == 0) {
                        using std::to_string;
                        execl("WNode", to_string(id).c_str(), to_string(port).c_str(), (char*)NULL);
                    }
                    break;
            }
        }
        void New(int inId) {
            id = inId;
            port = TakePort(sock);
            pid = fork();
            if (pid == 0) {
                using std::to_string;
                execl("WNode", to_string(id).c_str(), to_string(port).c_str(), (char*)NULL);
            }
        }
        void TakePortSetId(int Id) {
            id = Id;
            port = TakePort(sock);
        }
        void connectTo(int inPort) {
            sock.disconnect(ConURLPort(port));
            port = inPort;
            sock.connect(ConURLPort(port));
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
            return sock;
        }

    private:
        int id;
        int port;
        pid_t pid;
        zmq::context_t ctx;
        zmq::socket_t sock;
};

#endif