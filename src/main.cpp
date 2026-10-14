#include "myadd.h"
#include "myswap.h"
#include <iostream>
using namespace std;

int main() {
    int a = 5;
    int b = 10;
    myswap(a, b);
    int c = myadd(a, b);
    cout<<"c = a + b = "<<c<<endl;
    return 0;
}