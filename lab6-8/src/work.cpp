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
	Node Parent(Node::PARENT, 0, atoi(argv[1]));
    Node* Child = new Node;

    message to;
    message from;

    //for exec
    auto start = std::chrono::system_clock::now();
    auto stop = std::chrono::system_clock::now();
    int timerStat = 0;

    //
    
    while(true) {
        from.recv(Parent);
        switch (from.type) {
            case message::CREATE:
                if (Id == from.data) {
                    if (Child->Id() == -2) {
                        Child->New(from.id);
                        from.type = message::REPLY;
                        from.data = Child->Pid();
                        from.send(Parent);
                    } else {
                        Node* tmp = new Node(Node::NEW, 0, from.id);
                        to.type = message::TAKEPORT;
                        to.id = from.id;
                        to.send(*tmp);
                        from.recv(*tmp);
                        if (from.type != message::ERR) {
                            to.type = message::REPAR;
                            to.data = from.data;
                            to.send(*Child);
                            std::swap(tmp, Child);
                            delete tmp;

                            from.type = message::REPLY;
                            from.data = Child->Pid();
                            from.send(Parent);
                        } else {
                            std::cout << "err in WNode Create\n";
                        }
                    }  
                } else {
                    from.send(*Child);
                    to.recv(*Child);
                    to.send(Parent);
                }
                break;
            case message::TAKEPORT:
                Child->TakePortSetId(from.id);
                from.type = message::REPLY;
                from.data = Child->Port();
                puts("Sanding");
                from.send(Parent);
                break;
            case message::REPAR:
                Parent.connectTo(from.data);
                break;
            case message::TERM:
                if (Child->Id() != -2) {
                    from.send(*Child);
                }
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
                            from.send(Parent);
                            break;
                        case message::STOP:
                            if (timerStat == 1) {
                                stop = std::chrono::system_clock::now();
                                timerStat = 0;
                            }
                            from.type = message::REPLY;
                            from.send(Parent);
                            break;
                        case message::TIME:
                            from.type = message::TIME;
                            if (timerStat == 1) {
                                stop = std::chrono::system_clock::now();
                            }
                            auto msS = std::chrono::time_point_cast<std::chrono::milliseconds>(start).time_since_epoch().count();
                            auto msF = std::chrono::time_point_cast<std::chrono::milliseconds>(stop).time_since_epoch().count();
                            from.data = msF - msS;
                            from.send(Parent);
                            break;
                    }
                } else {
                    from.send(*Child);
                    to.recv(*Child);
                    to.send(Parent);
                }
                break;
        }
    }
	return 0;
}
