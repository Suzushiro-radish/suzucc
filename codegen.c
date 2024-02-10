#include "suzucc.h"

/**
 * raxにPUSHする
 */
static void push(void)
{
    printf("  push rax\n");
}

/**
 * 任意のレジスタにpopする
 */
static void pop(char *arg)
{
    printf("  pop %s\n", arg);
}

/**
 *  左辺値としての変数を生成する
 */
void gen_lval(Node *node)
{
    if (node->kind != ND_LVAR)
    {
        error("代入の左辺値が変数ではありません");
    }

    int offset = (node->name - 'a' + 1) * 8;
    printf("  lea rax, [rbp-%d]\n", offset);
    return;
}

void gen(Node *node)
{
    switch (node->kind)
    {
    case ND_NUM:
        printf("  mov rax, %d\n", node->val);
        return;
    case ND_LVAR:
        gen_lval(node);
        printf("  mov rax, [rax]\n");
        return;
    case ND_ASSIGN:
        gen_lval(node->lhs);
        push();
        gen(node->rhs);
        pop("rdi");
        printf("  mov [rdi], rax\n");
        return;
    }

    gen(node->rhs);
    push();
    gen(node->lhs);
    pop("rdi"); // rhsをrdiにpop

    switch (node->kind)
    {
    case ND_ADD:
        printf("  add rax, rdi\n");
        return;
    case ND_SUB:
        printf("  sub rax, rdi\n");
        return;
    case ND_MUL:
        printf("  imul rax, rdi\n");
        return;
    case ND_DIV:
        printf("  cqo\n");
        printf("  idiv rdi\n");
        return;
    case ND_EQ:
        printf("  cmp rax, rdi\n");
        printf("  sete al\n");
        printf("  movzb rax, al\n");
        return;
    case ND_NE:
        printf("  cmp rax, rdi\n");
        printf("  setne al\n");
        printf("  movzb rax, al\n");
        return;
    case ND_LT:
        printf("  cmp rax, rdi\n");
        printf("  setl al\n");
        printf("  movzb rax, al\n");
        return;
    case ND_LE:
        printf("  cmp rax, rdi\n");
        printf("  setle al\n");
        printf("  movzb rax, al\n");
        return;
    }

    error("invalid expression");
}

void gen_stmt(Node *node)
{
    if (node->kind == ND_STMT)
    {
        gen(node->lhs);
        return;
    }
    else
    {
        error("式ではありません");
    }
}

void codegen(Node *node)
{
    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");

    // プロローグ
    // 変数26個分の領域を確保する
    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp, %d\n", 26 * 8);

    for (Node *n = node; n; n = n->next)
    {
        gen_stmt(n);
    }

    // エピローグ
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
}