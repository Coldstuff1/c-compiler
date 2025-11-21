#include "minic-stdlib.h"

int binarySearch(int* a, int beg, int end, int val)
{
    int mid;
    if(end >= beg)
    {        mid = (beg + end)/2;
/* if the item to be searched is present at middle */
        if(a[mid] == val)
        {
            return mid+1;
        }
            /* if the item to be searched is smaller than middle, then it can only be in left subarray */
        else if(a[mid] < val)
        {
            return binarySearch(a, mid+1, end, val);
        }
            /* if the item to be searched is greater than middle, then it can only be in right subarray */
        else
        {
            return binarySearch(a, beg, mid-1, val);
        }
    }
    return -1;
}
void main() {
  int i; int a[9]; int val; int n; int res; int* b;
  a[0] = 11;
  a[1] = 14;
  a[2] = 25;
  a[3] = 30;
  a[4] = 40;
  a[5] = 41;
  a[6] = 52;
  a[7] = 57;
  a[8] = 70;
  val = 40; // value to be searched
  n = 9; // size of array
  b = (int*)a;
  res = binarySearch(b, 0, n-1, val); // Store result
  print_s((char*)"The elements of the array are -");
  //for (int i = 0; i < n; i++)
  i = 0;
  while(i < n) {
    print_i(a[i]);
    i = i+1;
  }
  print_s((char*)"\nElement to be searched is - ");
  print_i(val);
  if (res == -1){
  print_s((char*)"\nElement is not present in the array");
  }
  else {
  print_s((char*)"\nElement is present at ");
  print_i(res);
  print_s((char*)" position of array");
  }
}