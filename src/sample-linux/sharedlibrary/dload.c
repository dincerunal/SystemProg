#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

void exit_sys(const char *msg);

typedef int (*PADD)(int, int);
typedef int (*PMUL)(int, int);

int main(void)
{
    void *handle;
    PADD padd;
    PMUL pmul;
    
    handle = dlopen("./libmylib.so", RTLD_LAZY);
    
    if (handle == NULL) {
        fprintf(stderr, "dlopen:%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    
    padd = (PADD) dlsym(handle, "add");
    
    if (padd == NULL)  {
        fprintf(stderr, "dlsym:%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    
    pmul = (PMUL) dlsym(handle, "mul");
    
    if (pmul == NULL)  {
        fprintf(stderr, "dlsym:%s\n", dlerror());
        exit(EXIT_FAILURE);
    }
    
    printf("%d\n", padd(10, 20));
    printf("%d\n", pmul(10, 20));
    
    dlclose(handle);
    
    return 0;
}

void exit_sys(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}