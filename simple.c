#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

int token;
char *src, *old_src;
int poolsize;
int line;
int *text,
    *old_text,
    *stack;
char *data;
int *pc, *bp, *sp, ax, cycle;
enum { ADD, PUSH, LI, LC, SI, SC, IMM, EXIT};
void next(){
    token = *src++;
    return;
}

void program(){
    next();
    while (token > 0) {
        printf("token is: %c\n", token);
        next();
    }
};

int eval(){
    int op, *tmp;
    while (1) {
        op = *pc++;
        printf("%d, %p, %p\n", ax, (char *)ax, (int *)ax);
        if (op == IMM) { ax = *pc++;}
        else if (op == LC) { ax = *(char *)ax; }
        else if (op == LI) { ax = *(int *)ax; }
        else if (op == SC) { ax = *(char *)*sp++ = ax; }
        else if (op == SI) { *(int *)*sp++ = ax; }
        else if (op == CALL) { *--sp = (int)(pc + 1); pc = (int *)*pc; }
        else if (op == ENT) { *--sp = (int)bp; bp = sp; sp = sp - *pc++; }
        else if (op == ADJ) { sp = sp + *pc++; }
        else if (op == LEV) { sp = bp; bp = (int *)*sp++; pc = (int *)*sp++; }
        else if (op == LEA) { ax = (int)(bp + *pc++); }
        else if (op == PUSH) {*--sp = ax; }
        else if (op == ADD) ax = *sp++ + ax; 
        else if (op == EXIT) {printf("exit(%d)", *sp); return *sp;}
        else {
            printf("unknown instruction:%d\n", op);
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    int i, fd;

    argc--;
    argv++;

    poolsize = 256 * 1024;
    line = 1;
    if ((fd = open(*argv, 0)) < 0) {
        printf("could not open(%s)\n", *argv);
        return -1;
    }

    
    if (!(src = old_src = malloc(poolsize))) {
        printf("could not malloc(%d) for source area\n", poolsize);
        return -1;
    }

    if ((i = read(fd, src, poolsize - 1)) <= 0) {
        printf("read() returned %d\n", i);
        return -1;
    }
    if (!(text = old_text = malloc(poolsize))) {
        printf("could not malloc(%d) for text area\n", poolsize);
        return -1;
    }
    if (!(data = malloc(poolsize))) {
        printf("could not malloc(%d) for data area\n", poolsize);
        return -1;
    }

    if (!(stack = malloc(poolsize))) {
        printf("could not malloc(%d) for stack area\n", poolsize);
        return -1;
    }

    memset(stack, 0, poolsize);
    memset(text, 0, poolsize);
    memset(data, 0, poolsize);
    bp = sp = (int *)((int)stack + poolsize);
    ax = 0;

    i = 0;
    text[i++] = IMM;
    text[i++] = 10;
    text[i++] = PUSH;
    text[i++] = IMM;
    text[i++] = 20;
    text[i++] = ADD;
    text[i++] = PUSH;
    text[i++] = EXIT;
    pc = text;
    int sum;
    int a = 255;
    char x = 'c';
    int *b;
    b = &x;
    sum = *b;
    sum = *(int *)&sum;
    printf("%d\n", sum);
    sum = *(char *)&sum;
    printf("%c\n", sum);
    program();
    return eval();
    
}
