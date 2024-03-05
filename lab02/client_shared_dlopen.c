#include <stdio.h>
#include <dlfcn.h>

int main() {
    int (*collatz_conjecture)(int input);
    int (*test_collatz_convergence)(int input, int max_iter);

    void* dll_handle = dlopen("collatzlib/build/libcollatz_shared.so", RTLD_LAZY);
    if (!dll_handle) {
        fprintf(stderr, "Error: %s\n", dlerror());
        return 1;
    }

    collatz_conjecture = dlsym(dll_handle, "collatz_conjecture");
    test_collatz_convergence = dlsym(dll_handle, "test_collatz_convergence");

    printf("%d\n", collatz_conjecture(10));
    printf("%d\n", test_collatz_convergence(10, 100));
}