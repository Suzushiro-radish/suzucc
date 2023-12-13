#include "suzucc.h"

Node *new_node(NodeKind kind, Node *lhs, Node *rhs)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val)
{
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

// Consume the current token if it matches "op".
bool equal(Token *token, char *op)
{
    return memcmp(token->loc, op, token->len) == 0 && op[token->len] == '\0';
}

// Ensure that the current token is "op".
Token *skip(Token *tok, char *op)
{
    if (!equal(tok, op))
        error_tok(tok, "expected '%s'", op);
    return tok->next;
}

Node *stmt();
Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *primary();
Node *unary();

Node *stmt(Token **rest, Token *tok)
{
    Node *node = expr(rest, tok);
    *rest = skip(*rest, ";");
    return node;
}

Node *expr(Token **rest, Token *tok)
{
    Node *node = equality(rest, tok);
    return node;
}

Node *equality(Token **rest, Token *tok)
{
    Node *node = relational(&tok, tok);

    while (1)
    {
        if (equal(tok, "=="))
        {
            node = new_node(ND_EQ, node, relational(&tok, tok->next));
        }
        else if (equal(tok, "!="))
        {
            node = new_node(ND_NE, node, relational(&tok, tok->next));
        }
        else
        {
            *rest = tok;
            return node;
        }
    }
}

Node *relational(Token **rest, Token *tok)
{
    Node *node = add(&tok, tok);
    while (1)
    {
        if (equal(tok, "<"))
        {
            node = new_node(ND_LT, node, add(&tok, tok->next));
        }
        else if (equal(tok, ">"))
        {
            node = new_node(ND_LT, add(&tok, tok->next), node);
        }
        else if (equal(tok, "<="))
        {
            node = new_node(ND_LE, node, add(&tok, tok->next));
        }
        else if (equal(tok, ">="))
        {
            node = new_node(ND_LE, add(&tok, tok->next), node);
        }
        else
        {
            *rest = tok;
            return node;
        }
    }
}

Node *add(Token **rest, Token *tok)
{
    Node *node = mul(&tok, tok);

    while (1)
    {
        if (equal(tok, "+"))
        {
            node = new_node(ND_ADD, node, mul(&tok, tok->next));
        }
        else if (equal(tok, "-"))
        {
            node = new_node(ND_SUB, node, mul(&tok, tok->next));
        }
        else
        {
            *rest = tok;
            return node;
        }
    }
}

Node *mul(Token **rest, Token *tok)
{
    Node *node = unary(&tok, tok);

    while (1)
    {
        if (equal(tok, "*"))
        {
            node = new_node(ND_MUL, node, unary(&tok, tok->next));
        }
        else if (equal(tok, "/"))
        {
            node = new_node(ND_DIV, node, unary(&tok, tok->next));
        }
        else
        {
            *rest = tok;
            return node;
        }
    }
}

Node *unary(Token **rest, Token *tok)
{
    if (equal(tok, "+"))
        return primary(rest, tok->next);
    if (equal(tok, "-"))
        return new_node(ND_SUB, new_node_num(0), primary(rest, tok->next)); // xを0-xに置き換える
    return primary(rest, tok);
}

Node *primary(Token **rest, Token *tok)
{
    if (equal(tok, "("))
    {
        Node *node = expr(&tok, tok->next);
        *rest = skip(tok, ")");
        return node;
    }

    if (tok->kind == TK_NUM) {
        Node *node = new_node_num(tok->val);
        *rest = tok->next;
        return node;
    }

    error_tok(tok, "Expression expected.");
}

Node *parse(Token *tok)
{
    Node head = {};
    Node *cur = &head;
    while (tok->kind != TK_EOF)
    {
        cur = cur->next = stmt(&tok, tok);
    }

    return head.next;
}