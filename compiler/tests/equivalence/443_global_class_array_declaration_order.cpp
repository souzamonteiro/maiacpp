struct Token {
    unsigned int type;
    const char* value;
    unsigned int length;
    unsigned int start;
    unsigned int end;
};

static Token tokens[2];
static unsigned int token_count = 0;
static char input[8] = "help";

extern "C" int main(void)
{
    Token* token = tokens + token_count;
    token->type = 33;
    token->value = input;
    token->length = 4;
    token->start = 0;
    token->end = 4;
    ++token_count;
    if (token_count != 1) return 1;
    if (token->type != 33) return 2;
    int scan = 0;
    while (';' == ';' && scan < 1) ++scan;
    if ('{' != '{' || scan != 1) return 4;
    if (token->value[0] != 'h') return 3;
    return 0;
}
