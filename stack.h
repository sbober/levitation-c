#ifndef STACK_H
#define STACK_H 1

typedef struct {
        int *st;    /* stack elements */
        int top;    /* top element */
        int len;    /* current number of elements */
        int max;    /* maximum number of elements */
} stack;

void stack_init(stack *st, int size);

void stack_push(stack *st, int x);

int stack_pop(stack *st);
int stack_isempty(stack *st);
void stack_print(stack *st);

#endif
