#include <map>
#include <string>

void f() {
    std::map<std::string, int> m;
    for (std::map<std::string, int>::const_iterator it = m.begin(); it != m.end(); ++it) {
    }
}
