#include <iostream>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>


int main() {

    int shm = shm_open("./tmpSM", O_CREAT | O_TRUNC | O_RDWR, S_IRWXU);
    if (shm == -1) {
        std::cout << errno << " shm_open failed\n";
        exit(-1);
    }
    int r = ftruncate(shm, sysconf(_SC_PAGE_SIZE));
    if (r != 0) {
        std::cout << "ftruncate failed\n";
        exit(-2);
    }
    int* shared = (int*)mmap(0, sysconf(_SC_PAGE_SIZE), PROT_WRITE|PROT_READ, MAP_SHARED, shm, 0);
    if (shared == MAP_FAILED) {
        std::cout << "mmap failed\n";
        exit(-3);
    }
    close(shm);

    int serverPid = fork();
    if (serverPid < 0) {
        std::cout << "Cannot create server procces\n";
        exit(-1);
    }else if (serverPid == 0) {
        std::cout << "Create server procces\n";
        execl("./server.out", std::to_string((size_t)shared).c_str(), (char*)NULL);
        std::cout << "Cannot execute program\n" << errno << "\n";
    }
    //int msgSize, inMsg;
    while (true) {
        //std::getline(std::cin, outStr);
        //int a = outStr.length();
        //outStr[a] = '\n';
        //outStr[a + 1] = '\0';
        //if (write(pipeFdToServ[1], outStr.c_str(), a + 1) == -1) {
        //    std::cout << "Cannot write to pipe\n";
        //    exit(-1);
        //}
        //read(pipeFdFromServ[0], &msgSize, sizeof(int));
        std::cout << "Mesage no: " << shared[0] << '\n';
        if (shared[0] == -2) {
            break;
        }
        for (int i = 1; i < shared[0] + 1; i++) {
            //read(pipeFdFromServ[0], &inMsg, sizeof(int));
            std::cout << shared[i] << '\n';
        }
    }
    shm_unlink("./tmpSM");
    return 0;
}

