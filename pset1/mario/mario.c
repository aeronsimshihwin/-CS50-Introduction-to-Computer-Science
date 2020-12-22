#include <cs50.h>
#include <stdio.h>

int main (void)
{
    int n = 0;

    do
    {
        printf("Height: ");
        n = get_int();
        if (n < 0 || n > 23)
        {
            printf("Height is between 0 to 23. Retry: \n");
        }
    }
    while (n < 0 || n > 23);

    for(int r = 1; r < n+1; r++)
    {
        for(int space = n - r; space > 0; space--)
        {
            printf(" ");
        }

        for(int hash = 0; hash < r + 1; hash++)
        {
            printf("#");
        }
            printf("\n");
    }
}