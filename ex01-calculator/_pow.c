#include "_pow.h"

long long _pow(int base, int exponent)
{
    long long res = 1;
    while (exponent > 0)
    {
        res *= base;
        exponent--;
    }

    return res;
}