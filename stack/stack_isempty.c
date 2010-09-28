#include "stack.h"

int stack_isempty(stack *st) {
        if (st->len <= 0) return 1;
        else return 0;
}
