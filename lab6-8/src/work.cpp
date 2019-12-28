//#include <iostream>
#include <zmq.hpp>
#include <string>
#include <vector>
#include <chrono>
//#include <map>
#include "Nodes.hpp"
#include "Message.hpp"



int main(int argc, char** argv) {
	if (argc != 2)
		return 0;

    int Id = atoi(argv[0]);
	Node* Parent = new Node(Node::PARENT, 0, atoi(argv[1]));
    Node* Child = new Node;
    Node* tmp;

    message to;
    message from;

    //for exec
    auto start = std::chrono::system_clock::now();
    auto stop = std::chrono::system_clock::now();
    int timerStat = 0;
    //
    
    while(true) {
        //std::cout << Id << "wainting\n";
        from.recv(*Parent);
        switch (from.type) {
            case message::CREATE:
                if (Id == from.data) {
                    if (Child->Id() == -2) {
                        Child->New(from.id);
                        from.type = message::REPLY;
                        from.data = Child->Pid();
                        from.send(*Parent);
                    } else {
                        tmp = new Node(Node::NEW, from.id);
                        to.type = message::TAKEPORT;
                        to.id = Child->Id();
                        to.send(*tmp);
                        from.recv(*tmp);
                        // if(!from.recvCheck(*tmp) {
                        //     std::cout << "Error: Node is unavailable\n";
                        //     break;
                        // }
                        if (from.type != message::ERR) {
                            to.type = message::REPAR;
                            to.data = from.data;
                            to.send(*Child);
                            std::swap(tmp, Child);
                            delete tmp;
                            from.type = message::REPLY;
                            from.data = Child->Pid();
                            from.send(*Parent);
                        } else {
                            std::cout << "err in WNode Create\n";
                        }
                        
                    }  
                } else {
                    from.send(*Child);
                    if(!to.recvCheck(*Child)) {
                        // to.type = message::ERR;
                        // to.send(*Parent);
                        break;
                    } else {
                        to.send(*Parent);
                    }
                }
                break;
            case message::REMOVE:
                if (Child->Id() == from.id){
                    //tmp = new Node(Node::NEW, 0);
                    tmp = new Node();
                    tmp->TakePortSetId(0);
                    to.type = message::KILLNPASS;
                    to.data = tmp->Port();
                    to.send(*Child);
                    //to.recv(*Child);
                    if(!to.recvCheck(*Child)) {
                        // to.type = message::ERR;
                        // to.send(*Parent);
                        to.type = message::TERM;
                        to.send(*tmp);
                        delete tmp;
                        break;
                    }
                    if (to.type != message::ERR) {
                        tmp->Id() = to.id;
                        std::swap(tmp, Child);
                        //to.type = message::TERM;
                        //to.send(*tmp);
                        delete tmp;
                    } else {
                        std::cout << "Error: while removing\n";
                    }
                    from.send(*Parent);
                    break;
                } else {
                    from.send(*Child);
                    if(!to.recvCheck(*Child)) {
                        // to.type = message::ERR;
                        // to.send(*Parent);
                        break;
                    } else {
                        to.send(*Parent);
                    }
                    break;
                }
            case message::KILLNPASS:
                if (Child->Id() != -2) {
                    to.type = message::REPAR;
                    to.data = from.data;
                    to.send(*Child);
                }
                to.type = message::REPLY;
                to.id = Child->Id();
                to.send(*Parent);
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                return 0;
                break;
            case message::TAKEPORT:
                //puts("Taking");
                Child->TakePortSetId(from.id);
                from.type = message::REPLY;
                from.data = Child->Port();
                from.send(*Parent);
                break;
            case message::REPAR:
                tmp = new Node(Node::PARENT, 0, from.data);
                std::swap(tmp, Parent);

                delete tmp;
                break;
            case message::TERM:
                if (Child->Id() != -2) {
                    from.send(*Child);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                return 0;
                break;
            case message::EXEC:
                if (Id == from.id) {
                    switch (from.data) {
                        case message::START:
                            timerStat = 1;
                            start = std::chrono::system_clock::now();
                            stop = std::chrono::system_clock::now();
                            from.type = message::REPLY;
                            from.send(*Parent);
                            break;
                        case message::STOP:
                            if (timerStat == 1) {
                                stop = std::chrono::system_clock::now();
                                timerStat = 0;
                            }
                            from.type = message::REPLY;
                            from.send(*Parent);
                            break;
                        case message::TIME:
                            from.type = message::TIME;
                            if (timerStat == 1) {
                                stop = std::chrono::system_clock::now();
                            }
                            auto msS = std::chrono::time_point_cast<std::chrono::milliseconds>(start).time_since_epoch().count();
                            auto msF = std::chrono::time_point_cast<std::chrono::milliseconds>(stop).time_since_epoch().count();
                            from.data = msF - msS;
                            from.send(*Parent);
                            break;
                    }
                } else {
                    from.send(*Child);
                    if(!to.recvCheck(*Child)) {
                        // to.type = message::ERR;
                        // to.send(*Parent);
                        break;
                    } else {
                        to.send(*Parent);
                    }
                    break;
                }
                break;
            case message::PING:
                if (Id == from.id) {
                    to.type = message::REPLY;
                    to.send(*Parent);
                } else {
                    from.send(*Child);
                    if(!to.recvCheck(*Child)) {
                        // to.type = message::ERR;
                        // to.send(*Parent);
                        break;
                    } else {
                        to.send(*Parent);
                    }
                }
                break;
        }
    }
	return 0;
}
