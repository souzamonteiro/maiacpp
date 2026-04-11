int g0;
static int g1 = 2;

typedef unsigned long ULong;

int main() {
    int a = 1;
    ULong b = 2;
    return (int)(a + (int)b + g1 - g0 - 3);
}
