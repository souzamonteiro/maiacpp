#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

namespace math {
  template <typename T>
  T max_value(T a, T b) { return a > b ? a : b; }
  
  template <typename T>
  T min_value(T a, T b) { return a < b ? a : b; }
}

class Calculator {
private:
  int accumulator;
public:
  Calculator() : accumulator(0) {}
  
  int add(int x) { return accumulator += x; }
  int multiply(int x) { return accumulator *= x; }
  int get_result() const { return accumulator; }
};

struct Node {
  int value;
  Node* next;
  Node(int v) : value(v), next(0) {}
};

int main() {
  // Template instantiation
  int max_val = math::max_value(42, 100);
  int min_val = math::min_value(42, 100);
  
  // Class usage
  Calculator calc;
  calc.add(5);
  calc.multiply(3);
  
  // Linked list
  Node* head = new Node(10);
  head->next = new Node(20);
  head->next->next = new Node(30);
  
  int sum = 0;
  for (Node* n = head; n; n = n->next) {
    sum += n->value;
  }
  
  std::cout << max_val << " " << min_val << " " 
            << calc.get_result() << " " << sum << std::endl;
  
  delete head->next->next;
  delete head->next;
  delete head;
  
  return 0;
}
