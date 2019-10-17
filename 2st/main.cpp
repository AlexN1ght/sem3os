#include <iostream>
#include <unistd.h>
#include <cstring>
#include <string>

int main() {
    int pipeFdToServ[2];
    int pipeFdFromServ[2];
    char outStr[100];
    pipe(pipeFdToServ);
    pipe(pipeFdFromServ);
    char inStr[100];
    int serverPid = fork();
    if (serverPid < 0) {
        std::cout << "Cannot create server procces\n";
        //exit -1;
    }else if (serverPid == 0) {
        std::cout << "Create server procces\n";
        dup2(pipeFdToServ[0], STDIN_FILENO);
        close(pipeFdToServ[1]);
        execl("./server.out", std::to_string(pipeFdFromServ[0]).c_str(),
                              std::to_string(pipeFdFromServ[1]).c_str(), (char*)NULL);
        std::cout << "error " << errno << "\n";
    }
    close(pipeFdToServ[0]);
    close(pipeFdFromServ[1]);
    int msgSize, inMsg;
    read(pipeFdFromServ[0], inStr, 6);
    std::cout << inStr << '\n';
    while (true) {
        gets(outStr);
        int a = strlen(outStr);
        outStr[a] = '\n';
        outStr[a + 1] = '\0';
        write(pipeFdToServ[1], outStr, strlen(outStr));
        read(pipeFdFromServ[0], &msgSize, sizeof(int));
        std::cout << msgSize << '\n';
        for (int i = 0; i < msgSize; i++) {
            read(pipeFdFromServ[0], &inMsg, sizeof(int));
            std::cout << inMsg << '\n';
        }
    }
    
    return 0;
}
