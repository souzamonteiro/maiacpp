#include <iostream>

struct Node {
    Node(int node_value, Node *node_next) : value(node_value), next(node_next) {
    }

    int value;
    Node *next;
};

int sum_list(const Node *head) {
    int total = 0;
    while (head != 0) {
        total += head->value;
        head = head->next;
    }
    return total;
}

int main() {
    Node third(9, 0);
    Node second(6, &third);
    Node first(3, &second);

    std::cout << "sum=" << sum_list(&first) << std::endl;
    return 0;
}