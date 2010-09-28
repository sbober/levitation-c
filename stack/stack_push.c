#include "stack.h"

void stack_push(stack *st, int x) {
        /* FIXME: warn about this! */
        if (st->len >= st->max) return;
        else {
		st->top++;
		st->st[ st->top ] = x;
                st->len++;
        }
}
