#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    float n;
    do
    {
        printf("O hai! How much change is owed?\n");
        n = get_float();
    }
    while (n < 0);

    int cents = round(n * 100);

    //number of quarters
    int a = 0;
    while (cents > 24)
    {
        a = a + 1;
        cents = cents - 25;
    }

    //number of dimes
    int b = 0;
    while (cents > 9)
    {
        b = b + 1;
        cents = cents - 10;
    }

    //number of nickels
    int c = 0;
    while (cents > 4)
    {
        c = c + 1;
        cents = cents - 5;
    }

    //number of pennies
    int d = 0;
    while (cents > 0)
    {
        d = d + 1;
        cents = cents - 1;
    }
    printf("No. of quarters: %i\n", a);
    printf("No. of dimes: %i\n", b);
    printf("No. of nickels: %i\n", c);
    printf("No. of pennies: %i\n", d);
    printf("Total No. of coins: %i\n", a+b+c+d);
}