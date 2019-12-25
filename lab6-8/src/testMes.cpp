#include <iostream>
#include <zmq.hpp>
#include <string>
#include <vector>
#include <map>
#include "Nodes.hpp"
#include "Message.hpp"

enum { 
    ERR, CREATE,
    REMOVE, EXEC,
    PING, 
};


int main(void) {
    message test(message::DEF, 228, 322);
    Node testNode(Node::NEW, 5);
    puts("start");
    test.send(testNode);
    puts("sended");
    test.recv(testNode);
    std::cout << test.type << ' ' << test.id << ' ' << test.data << '\n';
    getchar();
}