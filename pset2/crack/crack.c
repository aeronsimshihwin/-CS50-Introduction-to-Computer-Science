#define _XOPEN_SOURCE
#include <unistd.h>
#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int argc, string argv[])
{
    // check that user only input single command-line arg
    if(argc != 2)
    {
        printf("Only single command-line argument.\n");
        return 1;
    }
    char salt[3];
    string hash;
    hash = argv[1];
    // first two characters of salt exactly same as that of hash
    salt[0] = hash[0];
    salt[1] = hash[1];
    salt[2] = '\0';
    
        // for one-letter words
        // labelled i
        for (char i='A'; i<='z'; i++)
        {
            // user input one-letter so only 0th position is taken and 1st position is NULL
            char password[2];
            password[0]=i;
            password[1]='\0';
        
            // usage of #include <unistd.h> to find crypt password (cpassword)
            string cpassword = crypt(password, salt);
            // compare hash and crypt password, if not same repeat above
            if (strcmp(argv[1], cpassword) == 0)
            {
                printf("%s\n", password);
                return 0;
            }
        }
            // for two-letters words
            // labelled i and j respectively
            for (char i='A'; i<='z'; i++)
            {
                for (char j='A'; j<='z'; j++)
                {
                    // user input two-letters so only 0th position and 1st position are taken, 2nd position is NULL
                    char password[3];
                    password[0]=i;
                    password[1]=j;
                    password[2]='\0';
        
                    string cpassword = crypt(password, salt);
                    if (strcmp(argv[1],cpassword) == 0)
                    {
                        printf("%s\n", password);
                        return 0;
                    }
                }
            }
                // for three-letters words
                // labelled i, j and k respectively
                for (char i='A'; i<='z'; i++)
                {
                    for (char j='A'; j<='z'; j++)
                    {
                        for (char k='A'; k<='z'; k++)
                        {
                            // user input three-letters so only 0th position, 1st position and 2nd position are taken, 3rd position is NULL
                            char password[4];
                            password[0]=i;
                            password[1]=j;
                            password[2]=k;
                            password[3]='\0';
                            
                            string cpassword = crypt(password, salt);
                            if (strcmp(argv[1], cpassword) == 0)
                            {
                                printf("%s\n", password);
                                return 0;
                            }
                        }
                    }
                }
                    // for four-letters words
                    // labelled i, j, k and l respectively
                    for (char i='A'; i<='z'; i++)
                    {
                        for (char j='A'; j<='z'; j++)
                        {
                            for (char k='A'; k<='z'; k++)
                            {
                                for (char l='A'; l<='z'; l++)
                                {
                                    // user input four-letters so only 0th position, 1st position, 2nd position and 3rd position are taken, 4th position is NULL
                                    char password[5];
                                    password[0]=i;
                                    password[1]=j;
                                    password[2]=k;
                                    password[3]=l;
                                    password[4]='\0';
                                    
                                    string cpassword = crypt(password, salt);
                                    if (strcmp(argv[1], cpassword) == 0)
                                    {
                                        printf("%s\n", password);
                                        return 0;
                                    }   
                                }
                            }
                        }
                    }
}