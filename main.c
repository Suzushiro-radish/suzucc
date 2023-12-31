#include "suzucc.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "引数の個数が正しくありません\n");
        return 1;
    }

    // Tokenizing and parsing
    Token *token = tokenize(argv[1]);
    Node *node = parse(token);

    // Export asm
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    for (Node *n = node; n; n = n->next)
    {
        gen(n);
        printf("  pop rax\n");
    }

    printf("  ret\n");
    return 0;
}