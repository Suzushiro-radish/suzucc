#include "suzucc.h"

static char *current_input;

// エラー報告用の関数
void error(char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

// エラーの場所を報告する関数
void verror_at(char *loc, char *fmt, va_list ap)
{
    int pos = loc - current_input;
    fprintf(stderr, "%s\n", current_input);
    fprintf(stderr, "%*s", pos, "");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

void error_at(char *loc, char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    verror_at(loc, fmt, ap);
}

void error_tok(Token *tok, char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    verror_at(tok->loc, fmt, ap);
}

bool startswith(char *main_string, char *prefix)
{
    return memcmp(main_string, prefix, strlen(prefix)) == 0;
}

// 新しいトークンを作成して最後のトークンcurに繋げる
Token *new_token(TokenKind kind, char *start, char *end)
{
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->loc = start;
    tok->len = end - start;
    return tok;
}

Token *tokenize(char *p)
{
    current_input = p;
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p)
    {
        // スペースをスキップ
        if (isspace(*p))
        {
            p++;
            continue;
        }

        if (startswith(p, "==") || startswith(p, "!=") || startswith(p, "<=") || startswith(p, ">="))
        {
            cur = cur->next = new_token(TK_RESERVED, p, p + 2);
            p += 2;
            continue;
        }

        if (strchr("+-*/()<>;", *p))
        {
            cur = cur->next = new_token(TK_RESERVED, p, p + 1);
            p += 1;
            continue;
        }

        if (isdigit(*p))
        {
            cur = cur->next = new_token(TK_NUM, p, p); // ここではpをインクリメントしない
            char *q = p;
            cur->val = strtol(p, &p, 10);       // increment p here
            cur->len = p - q;
            continue;
        }

        error("Failed tokenizing");
    }

    cur = cur->next = new_token(TK_EOF, p, p);
    return head.next;
}
