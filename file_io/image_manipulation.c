#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    unsigned char r, g, b;
} Pixel;

typedef struct
{
     unsigned int width, height;
     Pixel *ImageArray; // this would be an array of the colors for the pixels in the image.
} Image;

int GetPixelIndex(int width, int height, int row, int column)
{
    if (column < 0 || column >= width)
    {
        fprintf(stderr, "You asked for a pixel index for column %d, but the only valid values are between 0 and %d\n", column, width-1);
        exit(EXIT_FAILURE);
    }
    if (row < 0 || row >= height)
    {
        fprintf(stderr, "You asked for a pixel index for row %d, but the only valid values are between 0 and %d\n", row, height-1);
        exit(EXIT_FAILURE);
    }
    return row*width + column;
}

void
ReadImage(char *filename, Image *img)
{
    fprintf(stderr, "Reading image \"%s\"\n", filename);

    FILE *f_in;
    f_in = fopen(filename, "r"); // Note: rb is for read binary, I think just r would work too.
    char magicNum[128];
    unsigned int width, height, maxval;
    fscanf(f_in, "%s\n%d %d\n%d\n", magicNum, &width, &height, &maxval); //begining of pixels

    img -> width = width;
    img -> height = height;

    img -> ImageArray = malloc(width * height * sizeof(Pixel));

    Pixel p;

    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            fread(&p, sizeof(Pixel), 1, f_in);
            int index = GetPixelIndex(width, height, row, col);
            img -> ImageArray[index] = p;
        }
    }
    fprintf(stderr, "Done reading image \"%s\"\n", filename);
}

void WriteImage(char *filename, Image *img)
{
    fprintf(stderr, "Writing image \"%s\"\n", filename);

    FILE *f_out;
    f_out = fopen(filename, "w");

    fprintf(f_out, "P6\n");
    fprintf(f_out, "%d %d\n", img -> width, img -> height);
    fprintf(f_out, "%d\n", 255);

    fwrite(img -> ImageArray, sizeof(char), (img -> height * img -> width * sizeof(Pixel)), f_out);
    // printf("This is the size: %lu\n", (img -> height * img -> width * sizeof(Pixel)));
    fclose(f_out);
    fprintf(stderr, "Done writing image \"%s\"\n", filename);
}

void YellowDiagonal(Image *input, Image *output)
{
    fprintf(stderr, "Executing YellowDiagonal\n");

    unsigned int width, height;

    width = input -> width;
    height = input -> height;

    output -> ImageArray = malloc(width * height * sizeof(Pixel));

    output -> width = width;
    output -> height = height;

    for (int row = 0; row < height; ++row)
    {
        for (int col = 0; col < width; ++col)
        {
            int i = GetPixelIndex(width, height, row, col);
            if (row==col)
            {
                output -> ImageArray[i].r = 255;
                output -> ImageArray[i].g = 255;
                output -> ImageArray[i].b = 0;
                /* This 255 / 255 / 0 is the color yellow, and it will set only
                the pixels that are in a diagonal line to this color. */
            }
            else
            {
                /* This is keeping all of the pixels that are not in a diag the same.*/
                output -> ImageArray[i] = input -> ImageArray[i];
            }
        }
    }
    fprintf(stderr, "Done executing YellowDiagonal\n");
    return;
}

void LeftRightCombine(Image *input1, Image *input2, Image *output)
{
    fprintf(stderr, "Executing LeftRightCombine\n");

    unsigned int width1, width2, output_width, height;

    width1 = input1 -> width;
    width2 = input2 -> width;
    height = input1 -> height;
    output_width = width1 + width2;

    output -> ImageArray = malloc((width1 * height * sizeof(Pixel)) 
                            + (width2 * height * sizeof(Pixel)));

    output -> width = output_width;
    output -> height = height;

    for (int row = 0; row < height; row++)
    {
        for (int col1 = 0; col1 < width1; col1++)
        {
            int i = GetPixelIndex(width1, height, row, col1);
            /* This i is going to be the index of the pixel for Image1. */

            int j = GetPixelIndex(output_width, height, row, col1);

            /* j is going to be the pixel index of the new output image and this will be 
            were we put the pixel from image1 at index i in the output ImageArray. 

            We do not offset the output array here because this is the first 
            image and it starts the output image. */

            output -> ImageArray[j].r = input1 -> ImageArray[i].r;
            output -> ImageArray[j].g = input1 -> ImageArray[i].g;
            output -> ImageArray[j].b = input1 -> ImageArray[i].b;     
        }   
    }
    for (int row = 0; row < height; row++)
    {
        for (int col2 = 0; col2 < width2; col2++)
        {
            int i = GetPixelIndex(width2, height, row, col2);
            /* This i is going to be the index of the pixel for Image2. */
            int j = GetPixelIndex(output_width, height, row, col2);

            /* j is going to be the pixel index of the new output image and this will be 
            were we put the pixel from image2 at index i in the output ImageArray. 

            Offset the output array by the width of the first image so it will
            start right when image1 finishes. */

            output -> ImageArray[j + width1].r = input2 -> ImageArray[i].r;  
            output -> ImageArray[j + width1].g = input2 -> ImageArray[i].g;
            output -> ImageArray[j + width1].b = input2 -> ImageArray[i].b;
        }
    }
    fprintf(stderr, "Done executing LeftRightCombine\n");
}

void
VerifyInput()
{
    FILE *f_in = fopen("2G_input.pnm", "r");
    if (f_in == NULL)
    {
        fprintf(stderr, "Cannot open 2G_input.pnm for reading.\n");
        exit(EXIT_FAILURE);
    }
    fclose(f_in);

    FILE *f_out = fopen("2G_output.pnm", "w");
    if (f_out == NULL)
    {
        fprintf(stderr, "Cannot open 2G_output.pnm for writing.\n");
        exit(EXIT_FAILURE);
    }
    fclose(f_out);
}

int main(int argc, char *argv[])
{
    VerifyInput();

    Image source_image;
    Image yellow_diagonal;
    Image left_right;

    ReadImage("2G_input.pnm", &source_image);
    WriteImage("output_of_read.pnm", &source_image);
    
//#if 0
    YellowDiagonal(&source_image, &yellow_diagonal);
    WriteImage("yellow_diagonal.pnm", &yellow_diagonal);

//#endif

//#if 0
    LeftRightCombine(&source_image, &yellow_diagonal, &left_right);
    WriteImage("2G_output.pnm", &left_right);
//#endif
    free(source_image.ImageArray);
    free(yellow_diagonal.ImageArray);
    free(left_right.ImageArray);
    return 0;
}

// sudo pacman -Sy imagemagick. --> This will allow us to display the image in the VM with the display <image.pnm>