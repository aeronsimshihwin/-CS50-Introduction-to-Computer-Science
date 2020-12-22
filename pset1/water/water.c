#include <cs50.h>
#include <stdio.h>

 int main(void)
{
 int bottle = 12;
 int num_bottles;

 printf("minutes: \n");
 int minutes = get_int();

if (minutes > 0)
 {         
  num_bottles = bottle * minutes;
  printf("bottles: %d\n", num_bottles);
 }
else
 {
  num_bottles = bottle * minutes;
  printf("retry: ");
 }
}