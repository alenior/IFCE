#include <devkit_picrobot.h>

int8 sum(int a, int8 b) {
   return a + b;
}

void main() {
   int8 x = 2, y = 3;
   
   while(TRUE) {
      x = sum(x, y);
   }
}
