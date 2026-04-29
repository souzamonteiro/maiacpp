#include <iostream>
#include <pthread.h>

struct ThreadArg {
    ThreadArg(int loop_count, int *shared_counter) : loops(loop_count), shared(shared_counter) {
    }

    int loops;
    int *shared;
};

void *worker(void *opaque) {
    ThreadArg *arg = static_cast<ThreadArg *>(opaque);
    for (int i = 0; i < arg->loops; ++i) {
        *arg->shared = *arg->shared + 1;
    }
    return 0;
}

int main() {
    pthread_t thread;
    int counter = 0;
    ThreadArg arg(5, &counter);

    pthread_create(&thread, 0, worker, &arg);
    pthread_join(thread, 0);

    std::cout << "counter=" << counter << std::endl;
    return 0;
}