/* C++98 translation phase 6: adjacent string literals form one string. */
struct Asset { const char* name; const char* text; };
static const Asset assets[] = {
    { "first", "line one\n"
               "line two\n" },
    { "second", "a" "" "b\"c\\d" }
};
static const char* runtime = "hello" /* comment between fragments */ " world";

int same(const char* left, const char* right)
{
    while (*left && *right) {
        if (*left++ != *right++) return 0;
    }
    return *left == *right;
}
int main()
{
    const char* local = "local " "text";
    if (!same(assets[0].text, "line one\nline two\n")) return 1;
    if (!same(assets[1].text, "ab\"c\\d")) return 2;
    if (!same(runtime, "hello world")) return 3;
    if (!same(local, "local text")) return 4;
    if (!same("call " "argument", "call argument")) return 5;
    if (sizeof("a" "b") != 3) return 6;
    return 0;
}
