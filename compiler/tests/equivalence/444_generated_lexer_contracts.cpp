typedef unsigned int Unit;
typedef Unit Count;

struct Instruction { int op; Unit first; Unit second; Unit next; };
typedef Instruction LexInstruction;
struct Pattern { const LexInstruction* code; Count count; Count entry; };
struct Events { void* context; int (*accept)(void*, Count); };

static const Instruction instructions[] = { {0, 0, 0, 0}, {1, 43, 0, 0} };
static const Pattern patterns[] = { {instructions, 2, 1} };

const Pattern* getPattern() { return patterns; }
const Pattern* identity(const Pattern* pattern) { return pattern; }
extern "C" unsigned int declaredOnly(const Pattern* pattern);
extern "C" int variadicOnly(const char* text, ...);

int accept(void* context, Count value)
{
    int* expected = (int*)context;
    return value == (Count)*expected;
}

int main()
{
    Events* eventPointer;
    const Pattern* pattern;
    const Instruction* instruction;
    int expected = 43;
    Events events = { &expected, accept };
    Instruction local;
    Count limit = (Count)-1;
    if (limit / 4 != 1073741823) return 1;
    if (!events.accept(events.context, 43)) return 2;
    eventPointer = &events;
    if (!eventPointer->accept(eventPointer->context, 43)) return 3;
    if (eventPointer->accept(eventPointer->context, 42)) return 4;
    pattern = patterns;
    if (pattern->count != 2 || pattern->entry != 1) return 5;
    instruction = pattern->code + pattern->entry;
    if (instruction->op != 1 || instruction->first != 43) return 6;
    if (getPattern()->entry != 1) return 7;
    if (getPattern()[0].code[1].first != 43) return 8;
    local.op = 0;
    Instruction* reference = &local;
    reference->op = 7;
    if (local.op != 7) return 9;
    if (identity(getPattern())->count != 2) return 10;
    return 0;
}
