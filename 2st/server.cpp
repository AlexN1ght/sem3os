#include <iostream>
#include "stack.h"
#include "sort.h"
#include <string>
#include <unistd.h>

int main(int argc, char* argv[]) {;
    int pipeFd[2];
    int buf;
    //char strIn[100];
    pipeFd[0] = std::stoi(argv[0]);
    pipeFd[1] = std::stoi(argv[1]);
    close(pipeFd[0]);
    char c;
    int st;
    int val;
    Stack *A[10];
    for (int i = 0; i < 10; i++) {
        A[i] = NULL;
    }
    write(pipeFd[1], "Yay:", 6);
    while (1){
        scanf("%c", &c);
        switch (c) {
            case 'c':
                scanf("%d", &st);
                if (st > 9 || st < 0) {
                    buf = -1;
                    write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                A[st] = stack_create();
                buf = 0;
                write(pipeFd[1], &buf, sizeof(int));
                break;
            case 'd':
                scanf("%d", &st);
                if (st > 9 || st < 0) {
                    buf = -1;
                    write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                if (A[st] != NULL) {
                    stack_delete(&A[st]);
                    buf = 0;
                    write(pipeFd[1], &buf, sizeof(int));
                } else {
                    buf = -1;
                    write(pipeFd[1], &buf, sizeof(int));
                }
                break;
            case 'i':
                scanf("%d", &st);
                if (st > 9 || st < 0) {
                    buf = -1;
                    write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                if (A[st] == NULL) {
                    buf = -1;
                    write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                while (scanf("%d", val) {
                    stack_push(A[st], val);
                    c = getchar();
                    if (c == '\n') {
                        break;
                    }
                }
                buf = 0;
                write(pipeFd[1], &buf, sizeof(int));
                break;
            case 'o':
                scanf("%d", &st);
                if (st > 9 || st < 0) {
                    buf = -1;
                    write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                if (A[st] == NULL) {
                    buf = -1;
                    write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                if (!stack_is_empty(A[st])) {
                    buf = 1;
                    write(pipeFd[1], &buf, sizeof(int));
                    buf = stack_pop(A[st]);
                    write(pipeFd[1], &buf, sizeof(int));
                } else {
                    buf = -1;
                    write(pipeFd[1], &buf, sizeof(int));
                }
                break;
            case 's':
                scanf("%d", &st);
                if (st > 9 || st < 0) {
                    buf = -1;
                    write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                if (A[st] == NULL) {
                    buf = -1;
                    write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                if (stack_size(A[st]) < 0) {
                    buf = -1;
                    write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                sort(A[st]);
                buf = 0;
                write(pipeFd[1], &buf, sizeof(int));
                break;
            case 'p':
                scanf("%d", &st);
                if (st > 9 || st < 0) {
                    buf = -1;
                    write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                if (A[st] == NULL) {
                    buf = -1;
                    write(pipeFd[1], &buf, sizeof(int));
                    break;
                }
                buf = stack_size(A[st]);
                write(pipeFd[1], &buf, sizeof(int));
                stack_print(A[st], pipeFd[1]);
                break;
            case 'q':
                for (int i = 0; i < 10; i++) {
                    if (A[i] != NULL) {
                        stack_delete(&A[i]);
                    }
                }
                return 0;
                break;
            //case ' ':
            //    break;
            //case '\n':            
            //    break;
            default:
                buf = -1;
                write(pipeFd[1], &buf, sizeof(int));
                break;
        }
        while(c != '\n') {
            c = getchar();
        }
    }
}

