/**
 * generate.c
 *
 * Generates pseudorandom numbers in [0,MAX), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */
 
#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// upper limit on range of integers that can be generated
#define LIMIT 65536

int main(int argc, string argv[])
{
    // TODO: number of command-line arguments must be equal to 2 or 3
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./generate n [s]\n");
        return 1;
    }

    // TODO: store the user's input as an integer n
    int n = atoi(argv[1]);

    // TODO: the seed is supplied and will convert it into int using 'atoi'
    if (argc == 3)
    {
        srand48((long) atoi(argv[2]));
    }
    else // seed not supplied
    {
        srand48((long) time(NULL));
    }

    // TODO: iterate through user's input and randomize using 'drand48', then print output
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}