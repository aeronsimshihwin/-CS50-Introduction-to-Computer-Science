/**
 * resizemore.c
 *
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize 'factor, f' 'name of a BMP to be resized' 'name of the resized version to be written on' \n");
        return 1;
    }
    // convert user's input for factor to float
    float f = atof(argv[1]);
    // ensure user's input for factor is within range
    if ((f <= 0.0)||(f > 100.0))
    {
        fprintf(stderr, "Factor f is a floating-point value within range 0.0 exclusive and 100.0 inclusive.");
        return 1;
    }
    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];
    // open input file, if NULL, tell user
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 3;
    }
    // open output file, if NULL, tell user
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }
    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    // rs for resized
    BITMAPFILEHEADER bf_rs;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);
    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    BITMAPINFOHEADER bi_rs;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    bf_rs = bf;
    bi_rs = bi;
    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format, input file should be 24-bit uncompressed BMP 4.0.\n");
        return 5;
    }

    // calculate resized Width and Height
    bi_rs.biWidth = (bi.biWidth * ceil(f));
    bi_rs.biHeight = (bi.biHeight * ceil(f)); // if user key in f<1, still a problem

    // determine padding and resized padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    int padding_rs = (4 - (bi_rs.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // set file size and resized file size
    bi.biSizeImage = bi.biWidth * abs(bi.biHeight) + padding * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof (BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER);
    bi_rs.biSizeImage = bi_rs.biWidth * abs(bi_rs.biHeight) * sizeof(RGBTRIPLE) + padding_rs * abs(bi_rs.biHeight);
    bf_rs.bfSize = bi_rs.biSizeImage + sizeof (BITMAPFILEHEADER) + sizeof (BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf_rs, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi_rs, sizeof(BITMAPINFOHEADER), 1, outptr);

    // iterate over infile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        // temporary storage
        RGBTRIPLE triple;
        // count rows copied until it reaches f
        int rowcount = 0;
        while (rowcount < f)
        {
            // iterate over pixels in scanline
            for (int j = 0; j < bi.biWidth; j++)
            {
                // count pixel copied until it reaches f
                int pixelcount = 0;
                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);
                while (pixelcount < f)
                {
                    // write RGB triple to outfile
                    fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    pixelcount++;
                }
            }
            // add padding_rs
            for (int k = 0; k < padding_rs; k++)
            {
                fputc(0x00, outptr);
            }
            // shift pointer to start of row after adding padding_rs
            if (rowcount < (f-1))
            {
                // fseek(inptr, -str(temp), from)
                fseek(inptr, -(bi.biWidth*sizeof(RGBTRIPLE)), SEEK_CUR);
            }
            rowcount++;
        }
        // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);
    }
    // close infile
    fclose(inptr);
    // close outfile
    fclose(outptr);
    // success
    return 0;
}