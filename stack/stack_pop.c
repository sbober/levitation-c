#include "stack.h"

int stack_pop(stack *st) {
        int x;

        /* FIXME: warn about this! */
        if (st->len <= 0);
        else {
                x = st->st[ st->top ];
                st->top--;
                st->len--;
        }

        return(x);
}
