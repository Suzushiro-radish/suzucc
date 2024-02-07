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

    printf("  mov rax, rbp\n");
    printf("  sub rax, %d\n", ('z' - node->name + 1) * 8);
    push();
}

void gen(Node *node)
{
    switch (node->kind)
    {
    case ND_NUM:
        printf("  push %d\n", node->val);
        return;
    case ND_LVAR:
        gen_lval(node);
        pop("rax");
        printf("  mov rax, [rax]\n");
        push();
        return;
    case ND_ASSIGN:
        gen_lval(node->lhs);
        gen(node->rhs);
        pop("rdi");
        pop("rax");
        printf("  mov [rax], rdi\n");
        push();
        return;
    default:
        break;
    }

    gen(node->lhs);
    gen(node->rhs);
    pop("rdi"); // rhsをrdiにpop
    pop("rax"); // lhsをraxにpop

    switch (node->kind)
    {
    case ND_ADD:
        printf("  add rax, rdi\n");
        break;
    case ND_SUB:
        printf("  sub rax, rdi\n");
        break;
    case ND_MUL:
        printf("  imul rax, rdi\n");
        break;
    case ND_DIV:
        printf("  cqo\n");
        printf("  idiv rdi\n");
        break;
    case ND_EQ:
        printf("  cmp rax, rdi\n");
        printf("  sete al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_NE:
        printf("  cmp rax, rdi\n");
        printf("  setne al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_LT:
        printf("  cmp rax, rdi\n");
        printf("  setl al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_LE:
        printf("  cmp rax, rdi\n");
        printf("  setle al\n");
        printf("  movzb rax, al\n");
    }

    printf("  push rax\n");
}

void gen_stmt(Node *node)
{
    if (node->kind == ND_STMT)
    {
        gen(node->lhs);
        printf("  pop rax\n");
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

    for (Node *n = node; n; n = n->next)
    {
        gen_stmt(n);
    }

    printf("  ret\n");
}