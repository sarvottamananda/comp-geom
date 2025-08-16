#ifndef STACH_H
#define STACK_H

#include "data.h"

typedef struct point s_type;

typedef struct s_node {
    s_type val;
    struct s_node *next;
} *Stack;

Stack s_init(void);
s_type s_top(Stack);
Stack s_pop(Stack);
Stack s_push(Stack, s_type);
bool s_isempty(Stack);
void s_finish(Stack);

void s_print(Stack);

#endif
