#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>


int main() {
    unsigned long long test;

    test = (unsigned long long)9223372036854775807;

    printf("%llu\n", test);

    return 0;
}
