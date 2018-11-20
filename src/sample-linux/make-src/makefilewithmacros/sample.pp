# 1 "sample.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "/usr/include/stdc-predef.h" 1 3 4
# 1 "<command-line>" 2
# 1 "sample.c"
# 1 "a.h" 1



void foo();
# 2 "sample.c" 2
# 1 "b.h" 1



void bar();
# 3 "sample.c" 2


int main(void)
{
    foo();
    bar();

    return 0;
}
