#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>

const int THREAD_COUNT = 10;
std::mutex NL_m;

template<typename L>
void start_thread(std::vector<std::thread>& threads, L&& task)
{
    for(auto&& thread: threads)
    {
        if(thread.joinable()) 
            continue;
        thread = std::thread(task);
        puts("New thread created");
        return;
    }

    for(auto&& thread: threads)
    {
        if(!thread.joinable())
            continue;
        puts("Waiting for other thread to finish");
        thread.join();
        puts("Done. New thread created");
        thread = std::thread(task);
        return;
    }
}

template <typename T>
int minPath(T& M, int nodes, int from, int to) {
    if (from == to)
        return 0;
    std::vector<std::thread> threads(THREAD_COUNT);
    std::queue<int> CL;
    std::queue<int> NL; 
    int visited[nodes]{0};
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
            CL.pop();
            start_thread(threads, [&, CN]{
                for (int i = 0; i < nodes; i++) {
                    if (M[CN][i] == 1) {
                        if (visited[i] == 0) {
                            visited[i] = 1;
                            if (i == to) {
                                end = 1;
                            } else {
                                NL_m.lock();
                                NL.push(i);
                                NL_m.unlock();
                            }
                        }
                    }
                }    
            });
        }
        for(auto&& thread: threads)
        {
            if(!thread.joinable())
                continue;
            thread.join();
        }
        swap(CL, NL);
    }
    return path;    
}

int main(int argc, char *argv[]) {
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
    std::cout << minPath(M, N, from, to) << std::endl;
    
    return 0;
}

