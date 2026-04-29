#include <iostream>

enum { NODES = 6, QUEUE_CAP = 16 };

void bfs(const int (&graph)[NODES][NODES], int start) {
    bool visited[NODES] = {false, false, false, false, false, false};
    int queue[QUEUE_CAP];
    int head = 0;
    int tail = 0;

    visited[start] = true;
    queue[tail++] = start;

    while (head < tail) {
        int node = queue[head++];
        std::cout << node << std::endl;
        for (int i = 0; i < NODES; ++i) {
            if (graph[node][i] && !visited[i]) {
                visited[i] = true;
                queue[tail++] = i;
            }
        }
    }
}

int main() {
    const int graph[NODES][NODES] = {
        {0, 1, 1, 0, 0, 0},
        {0, 0, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 0}
    };

    bfs(graph, 0);
    return 0;
}