#include "minic-stdlib.h"

struct arguments {
    char* inputNumber;
    int inBase;
    int outBase;

};

int z;

int main() {
int a; int b; int x; int y; int z;

struct arguments bruh;


bruh.inputNumber = (char*)"1234";

z = 1;
y = 2;
x = 3;
b = 4;


a = x+(b-z)*z/b+x;

return a+b*z;

}