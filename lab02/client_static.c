#include <stdio.h>
#include "collatzlib/collatz.h"

int main() {
    printf("%d\n", collatz_conjecture(10));
    printf("%d\n", test_collatz_convergence(10, 100));
}