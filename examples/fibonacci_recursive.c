#include "minic-stdlib.h"

int factorial(int n) {
   //base case
   if(n == 0) {
      return 1;
   } else {
      return n * factorial(n-1);
   }
}

int fibbonacci(int n) {
    int a; int b;
   if(n == 0)return 0;
   if(n == 1)return 1;
   return fibbonacci(n-1)+fibbonacci(n-2);
}
void main() {
   int n;
   int i;
    n = read_i();
   print_s((char*)"Factorial of ");
   print_i(n);
   print_s((char*)": ");
   print_i(factorial(n));
   print_c('\n');
   print_s((char*)"Fibbonacci of ");
   print_i(n);
   print_s((char*)": ");
   i = 0;
   while(i < n) {
    print_i(fibbonacci(i));
    print_c(' ');
    i = i + 1;
   }



}