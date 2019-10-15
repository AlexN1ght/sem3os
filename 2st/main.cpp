#include <iostream>
#include <unistd.h>
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
        dup2(pipeFdFromServ[1], STDOUT_FILENO);
        close(pipeFdFromServ[0]);
        //execl("./server.out", std::to_string(pipeFd[0]).c_str(), std::to_string(pipeFd[1]).c_str(), (char*)NULL);
        execl("./server.out", (char*)NULL);
        std::cout << "error " << errno << "\n";
    }
    close(pipeFdToServ[0]);
    close(pipeFdFromServ[1]);
    //dup2(STDOUT_FILENO, pipeFdFromServ[0]);
    std::cout << "Test "<< "\n";
    while (true) {
        std::cout << "Test 1 "<< "\n";
        gets(outStr);
        int a = strlen(outStr);
        outStr[a] = '\n';
        outStr[a + 1] = '\0';
        std::cout << "Test 2"<< "\n";
        write(pipeFdToServ[1], outStr, strlen(outStr));
        //close(pipeFdToServ[1]);
        //close(pipeFdFromServ[1]);
        //wait(NULL);
        std::cout << "Test 3"<< std::endl; 
        std::cout << "Test 5"<< std::endl;
        read(pipeFdFromServ[0], inStr, 1);
        //dup2(STDOUT_FILENO, pipeFdFromServ[0]);
        //wait(NULL);
        std::cout << "Test 4"<< std::endl;
        std::cout << inStr;
    }
    
    return 0;
}
