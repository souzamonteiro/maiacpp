enum Kind { FIRST = 3, SECOND, THIRD };
struct Item { int value; Item* next; };

void append(Item** tail, Item* item)
{
    (*tail)->next = item;
    *tail = item;
}
int main()
{
    Kind kind = FIRST;
    const unsigned int values[4] = { 3, 4 };
    Item first;
    Item second;
    Item* tail = &first;
    Item** alias = &tail;
    first.value = FIRST;
    first.next = 0;
    second.value = SECOND;
    second.next = 0;
    append(alias, &second);
    if (first.next != &second || tail != &second) return 1;
    if (values[0] != FIRST || values[1] != SECOND || values[2] != 0 || values[3] != 0) return 2;
    int sum = 0;
    for (int index = 0; index < 3; ++index) sum += index;
    for (int index = 0; index < 2; ++index) {
        for (int index = 0; index < 2; ++index) sum += index;
    }
    int index = THIRD;
    if (sum != 5 || index != 5 || kind != FIRST) return 3;
    return 0;
}
