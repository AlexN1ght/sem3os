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
    PING, EXIT, START, 
    STOP, TIME
};


int main(void) {
    std::vector<Node*> lists;
    std::map<int, int> NodesAdr;
    NodesAdr[-1] = -1;

    //for text menu
    std::string comId;
    std::map <std::string, int> command;
    command["create"] = CREATE;     command["remove"] = REMOVE;
    command["exec"] = EXEC;         command["ping"] = PING;
    command["q"] = EXIT;            command["start"] = START;
    command["stop"] = STOP;         command["time"] = TIME;
    //
    //for parsing
    int id, parent, listID;
    //
    message to;
    message from;
    //message parsing
    int exit = 0;
    puts("Start");
    while(!exit) {
        std::cin >> comId;
        switch (command[comId]) {
            case CREATE:
                int id, parent;
                std::cin >> id >> parent;
                if (NodesAdr.find(parent) == NodesAdr.end()) {
                    std::cout << "Error: Parent not found\n";
                    break;
                }
                if (NodesAdr.find(id) != NodesAdr.end()) {
                    std::cout << "Error: Already exists\n";
                    break;
                }
                if (parent == -1) {
                    lists.push_back(new Node(Node::NEW, id));
                    pid_t added = lists[lists.size() - 1]->Pid();
                    NodesAdr[id] = lists.size() - 1;
                    std::cout << "Ok: " << added << '\n';
                } else {
                    listID = NodesAdr[parent];
                    to.type = message::CREATE;
                    to.id = id;
                    to.data = parent;
                    to.send(*lists[listID]);
                    from.recv(*lists[listID]);
                    if (from.type != message::ERR) {
                        std::cout << "Ok: " << from.data << '\n';
                        NodesAdr[id] = NodesAdr[parent];
                    } else {
                        std::cout << "Error: Parent is unavailable\n";
                    }
                }
                break;

            case EXEC:
                std::cin >> id;
                std::cin >> comId;
                if (NodesAdr.find(id) == NodesAdr.end()) {
                    std::cout << "Error:id: Not found\n";
                    break;
                } 
                switch (command[comId]) {
                    case START:
                        to.data = message::START;
                        break;
                    case STOP:
                        to.data = message::STOP;
                        break;
                    case TIME:
                        to.data = message::TIME;
                        break;
                    default:
                        std::cout << "Error:id: Wrong param\n";
                        break;
                }
                listID = NodesAdr[id];
                to.type = message::EXEC;
                to.id = id;
                to.send(*lists[listID]);
                from.recv(*lists[listID]);
                if (from.type == message::TIME) {
                    std::cout << "Ok:" << id << ": " <<from.data << '\n';
                } else if (from.type == message::ERR) {
                    std::cout << "Error: Somthing wrong\n";
                } else {
                    std::cout << "Ok:" << id << '\n';
                }
                break;
            case EXIT:
                exit = 1;
                break;
            case ERR:
                std::cout << "Wrong Command\n";
                break;
        }
    }
    
    for (int i = 0; i < lists.size(); i++) {
        to.type = message::TERM;
        to.send(*lists[i]);
        delete lists[i];
    }
    puts("Stop");
    return 0;
}