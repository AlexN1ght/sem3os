#include <iostream>
#include <unistd.h>
#include <cstring>
#include <string>

int main() {
    int pipeFdToServ[2];
    int pipeFdFromServ[2];
    std::string outStr;
    if (pipe(pipeFdToServ) == -1) {
        std::cout << "Cannot create pipe\n";
        exit(-1);
    }
    if (pipe(pipeFdFromServ) == -1) {
        std::cout << "Cannot create pipe\n";
        exit(-1);
    }
    int serverPid = fork();
    if (serverPid < 0) {
        std::cout << "Cannot create server procces\n";
        exit(-1);
    }else if (serverPid == 0) {
        std::cout << "Create server procces\n";
        if (dup2(pipeFdToServ[0], STDIN_FILENO) == -1) {
            std::cout << "Cannot dep2\n";
            exit(-1);
        }
        close(pipeFdToServ[1]);
        execl("./server.out", std::to_string(pipeFdFromServ[0]).c_str(),
                              std::to_string(pipeFdFromServ[1]).c_str(), (char*)NULL);
        std::cout << "Cannot execute program\n" << errno << "\n";
    }
    close(pipeFdToServ[0]);
    close(pipeFdFromServ[1]);
    int msgSize, inMsg;
    while (true) {
        std::getline(std::cin, outStr);
        int a = outStr.length();
        outStr[a] = '\n';
        outStr[a + 1] = '\0';
        if (write(pipeFdToServ[1], outStr.c_str(), a + 1) == -1) {
            std::cout << "Cannot write to pipe\n";
            exit(-1);
        }
        read(pipeFdFromServ[0], &msgSize, sizeof(int));
        std::cout << "Mesage no: " << msgSize << '\n';
        if (msgSize == -2) {
            break;
        }
        for (int i = 0; i < msgSize; i++) {
            read(pipeFdFromServ[0], &inMsg, sizeof(int));
            std::cout << inMsg << '\n';
        }
    }
    return 0;
}

