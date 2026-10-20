struct Second;
struct First { int value; First* next; Second* other; };
struct Second { int value; First* other; };
static const char* label = "typed pointer";
static First* root;

int main()
{
    First first;
    Second second;
    first.value = 17;
    first.next = &first;
    first.other = &second;
    second.value = 23;
    second.other = &first;
    root = &first;
    if (root->next->value != 17) return 1;
    if (root->other->other != root) return 2;
    if (root->other->value != 23) return 3;
    if (label[0] != 't') return 4;
    return 0;
}
