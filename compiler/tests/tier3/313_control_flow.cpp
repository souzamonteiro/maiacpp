int main() {
    int acc = 0;
    for (int i = 0; i < 3; ++i) {
        if (i == 1) {
            continue;
        }
        acc += i;
    }

    switch (acc) {
        case 2:
            break;
        default:
            return 1;
    }

    while (acc > 0) {
        acc--;
    }

    do {
        acc++;
    } while (acc < 1);

    return acc == 1 ? 0 : 1;
}
