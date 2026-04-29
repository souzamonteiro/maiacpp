#include <iostream>

struct TreeNode {
    TreeNode(int node_value, TreeNode *left_node, TreeNode *right_node)
        : value(node_value), left(left_node), right(right_node) {
    }

    int value;
    TreeNode *left;
    TreeNode *right;
};

void preorder(const TreeNode *node) {
    if (node == 0) {
        return;
    }
    std::cout << node->value << std::endl;
    preorder(node->left);
    preorder(node->right);
}

int main() {
    TreeNode node4(4, 0, 0);
    TreeNode node3(3, 0, 0);
    TreeNode node2(2, &node4, 0);
    TreeNode node1(1, &node2, &node3);

    preorder(&node1);
    return 0;
}