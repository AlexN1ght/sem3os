#include <iostream>
#include <thread>
#include <mutex>
#include <list>
#include <vector>
#include <queue>

int THREAD_COUNT = 1;
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
int minPath(T M, int from, int to) {
    if (from == to)
        return 0;
    std::vector<std::thread> threads(THREAD_COUNT);
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
            CL.pop();
            start_thread(threads, [&, CN]{
                for (int i: M[CN]) {
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

