#include <cs50.h>
#include <ctype.h> //to change letters to upper case
#include <stdio.h>
#include <string.h> //to use strlen(s)

int main(void)
{
    //ask user for input
    string s=get_string();
    //make sure get_string returned a string
    if(s!=NULL)
    //create for loop to scan through each character
    for(int i=0; i<strlen(s); i++)
    {
        // if user inputs spaces from the start then increase i
        while(s[i]==' ')
        {
            i++;
        }
        printf("%c", toupper(s[i]));
        //after printing the letter after the space, if it is not a space after this letter, increase i again
        while(s[i]!=' ' && s[i]!='\0')
        {
            i++;
        }
    }
    printf("\n");
}