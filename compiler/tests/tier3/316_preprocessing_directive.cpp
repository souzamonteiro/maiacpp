// Parse test for preprocessing directives
#define TEST_CONSTANT 42

int helper() {
    return 0;
}

int main() {
    return TEST_CONSTANT - 42;
}
