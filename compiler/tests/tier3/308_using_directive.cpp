namespace N {
int f() { return 1; }
}

using namespace N;

int main() {
    return f() == 1 ? 0 : 1;
}
