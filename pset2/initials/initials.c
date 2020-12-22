#include <cs50.h>
#include <ctype.h> //to change letters to upper case
#include <stdio.h>
#include <string.h> //to use strlen(s)

int main(void)
{
    string s = get_string();
    printf("%c", toupper(s[0]));
    if(s!=NULL)
    {
        for(int i=0; i<strlen(s); i++)
        {
            if(s[i]==' ' && s[i+1]!='\0')
            {
                printf("%c", toupper(s[i+1]));
            }
        }
        printf("\n");
    }
}