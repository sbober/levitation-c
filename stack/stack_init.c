#include <stdlib.h>
#include "stack.h"

void stack_init(stack *st, int size) {
        st->st = (int *) malloc( sizeof(int) * (size + 1) );
        st->top = -1;
        st->len = 0;
        st->max = size;
}
