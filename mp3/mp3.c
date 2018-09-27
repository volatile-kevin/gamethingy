#include <stdio.h>
#include <stdlib.h>
/*MP3 ECE 220
GROUP MEMBERS:
Kevin - kwh2
Joe - joseal2
Daniel - dao3
------------------------------------
The algorithm uses a single for loop calculate the coefficients
of the selected row and prints them onto the screen.
The user inputs the row to be printed.
The output will be the coefficients of the row that is selected
The coefficients will be printed on screen with spaces inbetween each number
After all the coefficients are printed a new line is printed
*/
int main()
{
  int row; /*The value of row will be determined by user input*/
  //int k;
  int j;
  unsigned long coeff; /*The unsigned long is used here to prevent overflow*/

  printf("Enter a row index: "); /*User input prompt*/
  scanf("%d",&row); /*User row selection*/
  // Write your code here
    for(j = 0; j <= row; j++){ /*This loop goes through each coefficient and calculates it*/
      if (j==0)/*Every first coefficient is 1*/
    coeff = 1;
      else
    coeff = coeff*(row-j+1)/j; /*Formula for finding the current coefficient*/
      printf("%lu ", coeff); /*Prints the coefficient and a space after each number*/
    }
    printf("\n");


  return 0;
}
