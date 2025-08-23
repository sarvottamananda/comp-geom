#include <stdio.h>
#include <stdlib.h>

#include "data.h"
#include "stack.h"

void sn_print(s_type v) { pt_print(v); }

void s_print(Stack s) {
    struct s_node *p = s;

    printf("Stack : ");
    while (p != NULL) {
        sn_print(p->val);
        p = p->next;
    }
}

Stack s_init(void) { return (Stack)(NULL); }

Stack s_pop(Stack s) {
    Stack tmp = s->next;
    free(s);
    return tmp;
}

Stack s_push(Stack s, s_type v) {
    Stack tmp = (Stack)malloc(sizeof(struct s_node));
    tmp->val = v;
    tmp->next = s;
    return tmp;
}

s_type s_top(Stack s) { return s->val; }

bool s_isempty(Stack s) { return s == NULL; }

void s_finish(Stack s) {
    while (!s_isempty(s))
        s = s_pop(s);
}
