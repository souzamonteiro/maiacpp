namespace MathCore {
int add(int x, int y) {
  return x + y;
}
}

namespace M = MathCore;

int main() {
  return M::add(2, 3) == 5 ? 0 : 1;
}
