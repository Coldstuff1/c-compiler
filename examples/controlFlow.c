#include "minic-stdlib.h"

void println(char* string) {
    print_s(string);
    print_c('\n');
}

void main() {
int x; int y; int z;

x = 1;

y = 2;

if(x == 1 || y==2) {
print_s((char*)"stmt 1 correct\n");
}

if(x==1 && y==2) {
print_s((char*)"stmt 2 correct\n");
}

if(x==1 || y==3) {
print_s((char*)"stmt 3 correct\n");
}

if(x==2 && y==2) {
print_s((char*)"stmt 4 incorrect\n");
}
else {
print_s((char*)"stmt 4 correct\n");
}

if(x==2) {
print_s((char*)"stmt 5 incorrect 1\n");
}
else if(y == 3) {
print_s((char*)"stmt 5 incorrect 2\n");
}
else if(x==1) {
print_s((char*)"stmt 5 correct");
}


}