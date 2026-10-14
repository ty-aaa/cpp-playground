#include "myswap.h"

void myswap(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}