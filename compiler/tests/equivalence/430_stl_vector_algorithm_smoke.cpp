#include <vector>
#include <algorithm>
#include <iterator>
#include <functional>

static int plus_one_fn(int x) { return x + 1; }

int main() {
    std::vector<int> base;
    base.push_back(4);
    base.push_back(1);
    base.push_back(3);
    base.push_back(2);

    std::sort(base.begin(), base.end());

    std::vector<int> lifted;
    std::transform(base.begin(), base.end(), std::back_inserter(lifted), std::ptr_fun(plus_one_fn));

    if (lifted.size() != 4) return 1;
    if (lifted[0] != 2) return 2;
    if (lifted[1] != 3) return 3;
    if (lifted[2] != 4) return 4;
    if (lifted[3] != 5) return 5;

    return 0;
}
