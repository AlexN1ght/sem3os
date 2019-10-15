#include <iostream>
#include <string>
#include <unistd.h>

int main(int argc, char* argv[]) {;
    int pipeFd[2];
    pipeFd[0] = std::stoi(argv[0]);
    pipeFd[1] = std::stoi(argv[1]);
    close(pipeFd[1]);
    char inStr[10];
    read(pipeFd[0], inStr, 10);
    close(pipeFd[0]);
    std::cout << inStr << '\n';
    return 0;
}

