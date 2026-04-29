#include <iostream>

class RingQueue {
public:
    enum { CAPACITY = 5 };

    RingQueue() : head_(0), tail_(0), count_(0) {
    }

    bool enqueue(int value) {
        if (count_ == CAPACITY) {
            return false;
        }
        data_[tail_] = value;
        tail_ = (tail_ + 1) % CAPACITY;
        ++count_;
        return true;
    }

    bool dequeue(int &value) {
        if (count_ == 0) {
            return false;
        }
        value = data_[head_];
        head_ = (head_ + 1) % CAPACITY;
        --count_;
        return true;
    }

private:
    int data_[CAPACITY];
    int head_;
    int tail_;
    int count_;
};

int main() {
    RingQueue queue;
    int value = 0;

    queue.enqueue(4);
    queue.enqueue(8);
    queue.enqueue(12);

    while (queue.dequeue(value)) {
        std::cout << "deq=" << value << std::endl;
    }
    return 0;
}