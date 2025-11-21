#include "minic-stdlib.h"



void main() {
int arr[2][3]; char carr[3][1];
arr[0][0] = 1;
arr[0][1] = 5;
arr[0][2] = 4;

arr[1][0] = 50;
arr[1][1] = 100;
arr[1][2] = 200;

print_i(arr[0][0]);
print_c(' ');
print_i(arr[0][1]);
print_c(' ');
print_i(arr[0][2]);
print_c(' ');
print_i(arr[1][0]);
print_c(' ');
print_i(arr[1][1]);
print_c(' ');
print_i(arr[1][2]);

carr[2][0] = 'c';
print_c(carr[2][0]);

}