#include <iostream>
#include "stack.h"
#include "sort.h"
#include <string>
#include <unistd.h>

int main(int argc, char* argv[]) {
    int* shared;
    //int buf;
    //char strIn[100];
    shared = (int*)std::stoi(argv[0]);
    //pipeFd[1] = std::stoi(argv[1]);
    //close(pipeFd[0]);
    char c;
    int st;
    int val;
    Stack *A[10];
    for (int i = 0; i < 10; i++) {
        A[i] = NULL;
    }
    while (1){
        scanf("%c", &c);
        switch (c) {
            case 'c':
                scanf("%d", &st);
                if (st > 9 || st < 0) {
                    shared[0] = -1;
                    //write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                A[st] = stack_create();
                shared[0] = 0;
                //write(pipeFd[1], &buf, sizeof(int));
                break;
            case 'd':
                scanf("%d", &st);
                if (st > 9 || st < 0) {
                    shared[0] = -1;
                    //write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                if (A[st] != NULL) {
                    stack_delete(&A[st]);
                    shared[0] = 0;
                    //write(pipeFd[1], &buf, sizeof(int));
                } else {
                    shared[0] = -1;
                    //write(pipeFd[1], &buf, sizeof(int));
                }
                break;
            case 'i':
                scanf("%d", &st);
                if (st > 9 || st < 0) {
                    shared[0] = -1;
                    //write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                if (A[st] == NULL) {
                    shared[0] = -1;
                    //write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                while (scanf("%d", &val)) {
                    stack_push(A[st], val);
                    c = getchar();
                    if (c == '\n') {
                        break;
                    }
                }
                shared[0] = 0;
                //write(pipeFd[1], &buf, sizeof(int));
                break;
            case 'o':
                scanf("%d", &st);
                if (st > 9 || st < 0) {
                    shared[0] = -1;
                    //write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                if (A[st] == NULL) {
                    shared[0] = -1;
                    //write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                if (!stack_is_empty(A[st])) {
                    shared[0] = 1;
                    //write(pipeFd[1], &buf, sizeof(int));
                    shared[1] = stack_pop(A[st]);
                    //write(pipeFd[1], &buf, sizeof(int));
                } else {
                    shared[0] = -1;
                    //write(pipeFd[1], &buf, sizeof(int));
                }
                break;
            case 's':
                scanf("%d", &st);
                if (st > 9 || st < 0) {
                    shared[0] = -1;
                    //write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                if (A[st] == NULL) {
                    shared[0] = -1;
                    //write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                if (stack_size(A[st]) < 0) {
                    shared[0] = -1;
                    //write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                sort(A[st]);
                shared[0] = 0;
                //write(pipeFd[1], &buf, sizeof(int));
                break;
            case 'p':
                scanf("%d", &st);
                if (st > 9 || st < 0) {
                    shared[0] = -1;
                    //write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                if (A[st] == NULL) {
                    shared[0] = -1;
                    //write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                shared[0] = stack_size(A[st]);
                //write(pipeFd[1], &buf, sizeof(int));
                stack_print(A[st], shared);
                break;
            case 'q':
                for (int i = 0; i < 10; i++) {
                    if (A[i] != NULL) {
                        stack_delete(&A[i]);
                    }
                }
                shared[0] = -2;
                //write(pipeFd[1], &buf, sizeof(int));
                return 0;
                break;
            default:
                shared[0] = -1;
                //write(pipeFd[1], &buf, sizeof(int));
                break;
        }
        while(c != '\n') {
            c = getchar();
        }
    }
}

