#include <cs50.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    int n = 0;

    do
    {
        printf("Full pyramid’s height: ");
        n=get_int();
        if(n < 0 || n > 23)
        {
            printf("Full pyramid’s height is a non-negative integer no greater than 23. Retry. ");
        }
    }
    while(n < 0 || n > 23);

    for(int r=0; r<n; r++)
    {
        for(int space=(n-r)-1; space>0; space--)
        {
            printf(" ");
        }

        for(int hash=0; hash<r+1; hash++)
        {
            printf("#");
        }

        printf("  ");

        for(int hash=0; hash<r+1; hash++)
        {
            printf("#");
        }
        printf("\n");
    }
}