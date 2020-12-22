#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, string argv[])  
{
    // have to be single command-line argument
    if (argc != 2)
    {
        printf("Incorrect number of arguments. Try again.\n");
        return 1;
    }
    // Convert string into integer
    int k=atoi(argv[1]);

    printf("plaintext: ");
    string p=get_string();

    printf("ciphertext: ");
    for(int i = 0, n=strlen(p); i<n; i++)
    {
        char c=p[i];
        
        if(islower(c))
        {
            char x=(((c-97)+k)%26)+97;
            printf("%c",x);
        }
        else if(isupper(c))
        {
        	char y=(((c-65)+k)%26)+65;
        	printf("%c",y);
        }
        else
        {
        	printf("%c",c);
        }
    }
	printf("\n");
    return 0;
}