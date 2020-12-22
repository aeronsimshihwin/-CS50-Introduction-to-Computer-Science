#include <stdio.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: ./recover 'name of a forensic image yo recover JPEGs'\n" );
        return 1;
    }
    char *fi = argv[1];
    FILE *inptr = fopen("card.raw", "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", fi);
        return 2;
    }
    //define outptr here to fopen later
    FILE *outptr = NULL;

    // filename: char array to store the resultant string; 002.jpg
    char JPEG[8];

    // to keep track of number of JPEG files written
    int JPEGcount = 0;

    // store 512B blocks in an array, googled how to select data structure to hold buffer in JPEG
    unsigned char buffer[512];

    // need to read 512 bytes at a time
    // fread(buffer, 1, 512, inptr) == 512 'cos once reach the end, !=512 and will jump out of loop
    while(fread(buffer, 1, 512, inptr) == 512)
    {
        // find beginning of JPEG
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            sprintf(JPEG, "%03i.jpg", JPEGcount);

            //MUST NOT CREATE OUTPTR AGAIN 'COS WAS NULL EARLIER ALRD
            //open JPEG file to be written
            outptr = fopen(JPEG, "w");

            // need to write 512 bytes at a time until a new JPEG is found
            fwrite(buffer, 1, 512, outptr);

            JPEGcount++;
        }
        else
        {
            if(outptr != NULL)
            {
                fwrite(buffer, 1, 512, outptr);
            }
        }
    }
    // close infile
    fclose(inptr);
    // success
    return 0;
}