#include <stdio.h>

int main() {
  int a = 0;
  int b = 0;
  sscanf("7 8", "%d %d", &a, &b);
  return b == 8 ? 0 : 1;
}
