/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */
 
#include <cs50.h>
#include <stdio.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    // TODO: implement a searching algorithm O(log n)
    if(n<0) // n is non-positive
    {
        return false;
    }
    else
    {
        int first=0;
        int last=n;
        while(first<last) // length of list more than zero
        {
            int middle = (first+last)/2; // look at middle of list
            if (value == values[middle])
            {
                printf("Found needle in haystack!");
                return true;
            }
            else if (value < values[middle])
            {
                last=middle-1; // shift last to middle + 1 to reduce scope
            }
            else
            {
                first=middle+1; // shift first to middle + 1 to reduce scope
            }
        }
    }
    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    // TODO: implement a sorting algorithm O(n^2)
    for(int i=0; i<n-1; i++)
    {
        for(int j=0; j<n-1-i; j++) /** need this row because after pushing 1 value 
                                    *  to the last, no need to use it to compare to others
                                    */
        
        {
            if(values[j] > values[j+1])
            {
                int temp=values[j];
                values[j]=values[j+1];
                values[j+1]=temp;
            }
        }
    }
}