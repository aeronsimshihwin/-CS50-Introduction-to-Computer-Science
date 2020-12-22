#include <cs50.h>
#include <stdio.h>

int main(void)
{
    long long c;
    
    do
    {
        printf("Credit card number: ");
        c=get_long_long();
    }
    while (c<0);
    
    if((c>=340000000000000 && c<350000000000000) || (c>=370000000000000 && c<380000000000000))
    {
        printf("AMEX\n");
    }
    else if(c>=5100000000000000 && c<5600000000000000)
    {
        printf("MASTERCARD\n");
    }
    else if ((c>=4000000000000 && c<5000000000000) || (c>=4000000000000000 && c<5000000000000000))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
}
