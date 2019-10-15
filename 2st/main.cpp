#include <iostream>
#include <unistd.h>
#include <string>

int main() {
    int pipeFd[2];
    pipe(pipeFd);
    char strOut[100];
    int serverPid = fork();
    if (serverPid < 0) {
        std::cout << "Cannot create server procces\n";
        //exit -1;
    }else if (serverPid == 0) {
        std::cout << "Create server procces\n";
        execl("./server.out", std::to_string(pipeFd[0]).c_str(), std::to_string(pipeFd[1]).c_str(), (char*)NULL);
        std::cout << "error " << errno << "\n";
    }
    while (true) {
        outStr = gets();
        close(pipeFd[0]);
        write(pipeFd[1], outStr, 100);
        close(pipeFd[1]);
    }
    
    return 0;
}
