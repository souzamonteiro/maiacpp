class Node;

class Node {
public:
    int v;
};

Node* make_node(Node* p) {
    return p;
}

int main() {
    Node n;
    n.v = 1;
    return make_node(&n)->v == 1 ? 0 : 1;
}
