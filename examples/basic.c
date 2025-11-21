#include "minic-stdlib.h"

struct vector{
int x;
int y;
};

int addition(int x, int y) {
return x+y;
}

void main() {
int x; int y;struct vector v1; struct vector v2;

v1.x =3;
v1.y =4;
print_i(addition(v1.x,v1.y));
print_s((char*)"\n");
v2 = v1;
print_i(v2.x);
print_c(' ');
print_i(v2.y);

}