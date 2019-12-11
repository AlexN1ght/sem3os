#include "stack.h"
#include "sort.h"
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>

typedef struct
{
    int stor[1024];
    pthread_mutex_t mutex;
} shared_data;

static shared_data* data = NULL;

void initialise_shared()
{
    int prot = PROT_READ | PROT_WRITE;
    int flags = MAP_SHARED | MAP_ANONYMOUS;
    data = (shared_data*)mmap(NULL, sizeof(shared_data), prot, flags, -1, 0);
    if (data == MAP_FAILED) {
        exit(-3);
    }

    pthread_mutexattr_t attr;
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&data->mutex, &attr);
}

int run_child()
{
    char c;
    int st;
    int val;
    Stack *A[10];
    for (int i = 0; i < 10; i++) {
        A[i] = NULL;
    }
    while (1){
        sleep(1);
        pthread_mutex_lock(&data->mutex);
        scanf("%c", &c);
        switch (c) {
            case 'c':
                scanf("%d", &st);
                if (st > 9 || st < 0) {
                    data->stor[0] = -1;
                    break;
                }
                A[st] = stack_create();
                data->stor[0] = 0;
                break;
            case 'd':
                scanf("%d", &st);
                if (st > 9 || st < 0) {
                    data->stor[0] = -1;
                    break;
                }
                if (A[st] != NULL) {
                    stack_delete(&A[st]);
                    data->stor[0] = 0;
                } else {
                    data->stor[0] = -1;
                }
                break;
            case 'i':
                scanf("%d", &st);
                if (st > 9 || st < 0) {
                    data->stor[0] = -1;
                    break;
                }
                if (A[st] == NULL) {
                    data->stor[0] = -1;
                    break;
                }
                while (scanf("%d", &val)) {
                    stack_push(A[st], val);
                    c = getchar();
                    if (c == '\n') {
                        break;
                    }
                }
                data->stor[0] = 0;
                break;
            case 'o':
                scanf("%d", &st);
                if (st > 9 || st < 0) {
                    data->stor[0] = -1;
                    break;
                }
                if (A[st] == NULL) {
                    data->stor[0] = -1;
                    break;
                }
                if (!stack_is_empty(A[st])) {
                    data->stor[0] = 1;
                    data->stor[1] = stack_pop(A[st]);
                } else {
                    data->stor[0] = -1;
                }
                break;
            case 's':
                scanf("%d", &st);
                if (st > 9 || st < 0) {
                    data->stor[0] = -1;
                    break;
                }
                if (A[st] == NULL) {
                    data->stor[0] = -1;
                    break;
                }
                if (stack_size(A[st]) < 0) {
                    data->stor[0] = -1;
                    break;
                }
                sort(A[st]);
                data->stor[0] = 0;
                break;
            case 'p':
                scanf("%d", &st);
                if (st > 9 || st < 0) {
                    data->stor[0] = -1;
                    break;
                }
                if (A[st] == NULL) {
                    data->stor[0] = -1;
                    break;
                }
                data->stor[0] = stack_size(A[st]);
                stack_print(A[st], data->stor);
                break;
            case 'q':
                for (int i = 0; i < 10; i++) {
                    if (A[i] != NULL) {
                        stack_delete(&A[i]);
                    }
                }
                data->stor[0] = -2;
                pthread_mutex_unlock(&data->mutex);
                return 0;
                break;
            default:
                data->stor[0] = -1;
                break;
        }
        while(c != '\n') {
            c = getchar();
        }
        pthread_mutex_unlock(&data->mutex);
    }
}

void run_parent()
{
    while (true) {
        sleep(1);
        pthread_mutex_lock(&data->mutex);
        std::cout << "Mesage no: " << data->stor[0] << '\n';
        if (data->stor[0] == -2) {
            break;
        }
        for (int i = 1; i <= data->stor[0]; i++) {
            //read(pipeFdFromServ[0], &inMsg, sizeof(int));
            std::cout << data->stor[i] << '\n';
        }
        pthread_mutex_unlock(&data->mutex);
    }
}

int main(int argc, char** argv)
{
    initialise_shared();

    pid_t serverPid = fork();
    if (serverPid < 0) {
        std::cout << "Cannot create server procces\n";
        exit(-1);
    } else if (serverPid == 0) {
        std::cout << "Create server procces\n";
        run_child();
    } else {
        run_parent();
    }

    munmap(data, sizeof(data));
    return 0;
}