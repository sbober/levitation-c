#include "libowfat/buffer.h"
#include "stack.h"

void stack_print(stack *st) {
        int i;

        for (i = st->top; i >= 0; i--) {
            buffer_putlong(buffer_1, st->st[i]);
            buffer_puts(buffer_1, " ");
        }
        buffer_putnlflush(buffer_1);
}
