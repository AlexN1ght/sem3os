#include <iostream>
#include <pthread.h>
#include <list>
#include <vector>
#include <queue>

int THREAD_COUNT = 1;
pthread_mutex_t NL_m;

struct Arg {
    std::vector<std::list<int>>* M;
    std::queue<int>* NL;
    int* visited;
    int CN;
    int* end;
    int to;
};

void* threadFunction(void *arg) {
    for (int i: ((Arg*)arg)->M->at(((Arg*)arg)->CN)) {
        if (((Arg*)arg)->visited[i] == 0) {
            ((Arg*)arg)->visited[i] = 1;
            if (i == ((Arg*)arg)->to) {
                *((Arg*)arg)->end = 1;
            } else {
                pthread_mutex_lock(&NL_m); 
                ((Arg*)arg)->NL->push(i);
                pthread_mutex_unlock(&NL_m); 
            }
        }
    }
    return NULL;
};

template <typename T>
int minPath(T M, int from, int to) {
    if (from == to)
        return 0;
    std::vector<pthread_t> threads(THREAD_COUNT);
    int threadIter = 0;
    Arg thArgs[THREAD_COUNT];
    std::queue<int> CL;
    std::queue<int> NL; 
    int visited[M.size()]{0};
    visited[from] = 1;
    int path = 0;
    int end = 0, closed = 1;
    CL.push(from);
    while (!end) {
        if (CL.empty())
            return -1;
        path++;
        puts("New layer");
        while (!CL.empty()) {
            int CN = CL.front();
            CL.pop();;
            if (threads[threadIter]) {
                puts("Waiting for other thread");
                pthread_join(threads[threadIter], NULL);
                //threads[threadIter] = 0;
            }
            thArgs[threadIter].CN = CN;
            thArgs[threadIter].end = &end;
            thArgs[threadIter].to = to;
            thArgs[threadIter].M = &M;
            thArgs[threadIter].visited = visited;
            thArgs[threadIter].NL = &NL;
            pthread_create(&threads[threadIter], NULL, threadFunction, &thArgs[threadIter]);
            puts("Thread created");
            threadIter = (threadIter + 1) % THREAD_COUNT;
            

        }
        puts("Waiting for all threads to stop");
        for(auto&& thread: threads)
        {
            if (thread == 0) 
                continue;
            pthread_join(thread, NULL);
            thread = 0;
        }
        
        swap(CL, NL);
    }
    return path;    
}

template <typename T>
auto toNoW(T& M, int N) {
    std::vector<std::list<int>> out(N);
    int add = N;
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            if (M[i][k] > 0) {
                if (M[i][k] == 1){
                    out[i].push_back(k);
                } else {
                    int rest = M[i][k] - 1;
                    out[i].push_back(add);
                    rest--;
                    while(rest--) {
                        out.push_back(std::list<int>{++add});
                    }
                    out.push_back(std::list<int>{k});
                    add++;
                }
                
            }
        }
    }
    std::cout << add - N << " nodes added\n";
    return out;
}

int main(int argc, char *argv[]) {
    if (argc == 2) 
        THREAD_COUNT = std::atoi(argv[1]);
    int N;
    std::cin >> N;
    int** M = new int*[N];
    for(int i = 0; i < N; i++)
        M[i] = new int[N];
    for (int i = 0; i < N; i++) {
        for (int k = 0; k < N; k++) {
            std::cin >> M[i][k];
        }
    } 
    int from, to;
    std::cin >> from >> to;
    auto ML = toNoW(M, N);
    for (int i = 0; i < ML.size(); i++) {
        std::cout << i << ": ";
        for (int k : ML[i]) {
            std::cout << k << ' ';
        }
        std::cout << '\n';
    }
    int min = minPath(ML, from, to);
    std::cout << "Total path: " << min << std::endl;
    delete[] M;
    return 0;
}

