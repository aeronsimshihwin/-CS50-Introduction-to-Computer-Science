#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])  
{
    // have to be single command-line argument
    if(argc != 2)
    {
        printf("Incorrect number of arguments. Try again.\n");
        return 1;
    }
    else
    {
        for(int i=0, n=strlen(argv[1]); i<n; i++)
        {
            // have to contain letters only
            if(!isalpha(argv[1][i]))
            {
                printf("Only alphabets, no numbers.");
                return 1;
            }
        }
    }
    
    //ask user for plaintext to encrypt
    printf("plaintext: ");
    string p=get_string();
    
    string k=argv[1];
    do
    {
        k=strcat(k, k);
    }
    while(strlen(k)<strlen(p));
    
    printf("ciphertext: ");
    //iterate through plaintext to encrypt
    int j = 0;
    for(int i=0, n=strlen(p); i<n; i++)
    {
        char c=p[i];
        {
            if(islower(c))
            {
                char x = (((p[i] - 'a') + (tolower(k[j]) - 'a'))%26) + 'a';
                printf("%c", x);
                j++;
            }                
            else if(isupper(c))
            {
                char y = (((p[i] - 'A') + (tolower(k[j]) - 'a'))%26) + 'A';
                printf("%c", y);
                j++;
            }
            else if(!isalpha(c))
            {
                printf("%c",p[i]);
            }
        }
    }
    printf("\n");
    return 0;
}