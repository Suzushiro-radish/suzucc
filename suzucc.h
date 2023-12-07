#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef enum
{
    TK_RESERVED,
    TK_NUM,
    TK_EOF,
} TokenKind;

typedef struct Token Token;
struct Token
{
    TokenKind kind; // トークンの種類
    Token *next;    // 次のトークンのポインタ
    int val;        // kindがTK_NUMの時の数値
    char *loc;      // トークンの場所
    int len;        // トークンの文字数
};

typedef enum
{
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_NUM,
    ND_EQ,
    ND_NE,
    ND_LT, // <
    ND_LE, // <=
} NodeKind;

typedef struct Node Node;

struct Node
{
    NodeKind kind;
    Node *lhs; // Left-hand side
    Node *rhs; // Right-hand side
    int val;
};

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
void error_tok(Token *tok, char *fmt, ...);

Token *tokenize(char *p);
void gen(Node *node);
Node *parse(Token *tok);
