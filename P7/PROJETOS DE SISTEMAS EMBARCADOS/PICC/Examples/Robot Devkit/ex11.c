#include <devkit_picrobot.h>
#include <ios.h>
#use rs232(DEBUGGER, xmit=PIN_B10, rcv=PIN_B10)

int8 sum(int a, int8 b) {
   return a + b;
}

void main() {
   long a, b, result;
   char opr;
   
   setup_timer2(TMR_INTERNAL | TMR_DIV_BY_1, 65534);
   
   while(TRUE) {
      cout << "\r\nEnter the first number: ";
      cin >> a;
      
      do {
         cout << "\r\nEnter the operator (-+/*): ";
         cin >> opr;
      } while (!isamoung(opr,"+-/*"));
      
      cout <<"\r\nEnter the second number: ";
      cin >> b;
      
      switch(opr) {
         case '+' : result = a+b; break;
         case '-' : result = a-b; break;
         case '*' : result = a*b; break;
         case '/' : result = a/b; break;
      }
      
      cout << "The result is: " << result << endl;
   }
}

