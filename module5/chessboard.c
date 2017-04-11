/*
 *  chessboard.c
 *  create a 512x512 BMP of a chessboard, with user specified size (in pixels)
 *  of the black and white squares on the board.  bottom right square must be white.
 *
 *  Created by Richard Buckland on 14/04/11, edited 5/4/14 by Andrew Timkov
 *  Licensed under Creative Commons BY 3.0.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
 
#define BYTES_PER_PIXEL 3
#define BITS_PER_PIXEL (BYTES_PER_PIXEL*8)
#define NUMBER_PLANES 1
#define PIX_PER_METRE 2835
#define MAGIC_NUMBER 0x4d42
#define NO_COMPRESSION 0
#define OFFSET 54
#define DIB_HEADER_SIZE 40
#define NUM_COLORS 0
#define MAX_RGB 255

#define BLACK 0
#define WHITE 0
#define SIZE 512
#define BMP_FILE "chess.bmp"
 
typedef unsigned char  bits8;
typedef unsigned short bits16;
typedef unsigned int   bits32;
 
void writeHeader (FILE *file);
void writeColor (bits8 data, int elementNum, FILE *outputFile);
 
int main (int argc, char *argv[]) {
 
   // check that the types have the size i'm relying on here
   assert (sizeof(bits8)  == 1);
   assert (sizeof(bits16) == 2);
   assert (sizeof(bits32) == 4);
 
   FILE *outputFile;
   int squareSize;
 
   outputFile = fopen(BMP_FILE, "wb");
   assert ((outputFile != NULL) && "Cannot open file");
 
   writeHeader(outputFile);
 
   printf ("Enter square size (must be a factor of %d): \n", SIZE);
   scanf ("%d", &squareSize);
   assert (SIZE % squareSize == 0);

   //calculate number of squares on each side
   int squaresOnSide = SIZE / squareSize;
   bits8 color;
   int row = 0;

   //iterate through rows of squares
   while (row < squaresOnSide) {
      int verticalPixel = 0;
      //iterate through vertical pixels of each square
      while (verticalPixel < squareSize) {
         int col = 0;
         //iterate through columns of squares
         while (col < squaresOnSide) {
            int horizontalPixel = 0;
            //iterate through horizontal pixels of each square
            while (horizontalPixel < squareSize) {
               //calculate color of the pixel
               //done by adding co-ords of square containing the pixel
               //then state if it is an odd or even square (modulus)
               //then multiplies by 255
               //0*255 is 0 (black), 1*255 is 255 (white)
               color = ((row + col) % 2) * MAX_RGB;
               writeColor(color, 1, outputFile);
               horizontalPixel++;
            }
            col++;
         }
         verticalPixel++;
      }
      row++;
   }

   fclose(outputFile);
 
   return EXIT_SUCCESS;
}

void writeColor (bits8 data, int elementNum, FILE *outputFile) {
   int i = 0;
   while (i < BYTES_PER_PIXEL) {
        fwrite(&data, sizeof data, elementNum, outputFile);
        i++;
   }

   return;
}
 
void writeHeader (FILE *file) {
   assert(sizeof (bits8) == 1);
   assert(sizeof (bits16) == 2);
   assert(sizeof (bits32) == 4);
 
   bits16 magicNumber = MAGIC_NUMBER;
   fwrite (&magicNumber, sizeof magicNumber, 1, file);
 
   bits32 fileSize = OFFSET + (SIZE * SIZE * BYTES_PER_PIXEL);
   fwrite (&fileSize, sizeof fileSize, 1, file);
 
   bits32 reserved = 0;
   fwrite (&reserved, sizeof reserved, 1, file);
 
   bits32 offset = OFFSET;
   fwrite (&offset, sizeof offset, 1, file);
 
   bits32 dibHeaderSize = DIB_HEADER_SIZE;
   fwrite (&dibHeaderSize, sizeof dibHeaderSize, 1, file);
 
   bits32 width = SIZE;
   fwrite (&width, sizeof width, 1, file);
 
   bits32 height = SIZE;
   fwrite (&height, sizeof height, 1, file);
 
   bits16 planes = NUMBER_PLANES;
   fwrite (&planes, sizeof planes, 1, file);
 
   bits16 bitsPerPixel = BITS_PER_PIXEL;
   fwrite (&bitsPerPixel, sizeof bitsPerPixel, 1, file);
 
   bits32 compression = NO_COMPRESSION;
   fwrite (&compression, sizeof compression, 1, file);
 
   bits32 imageSize = (SIZE * SIZE * BYTES_PER_PIXEL);
   fwrite (&imageSize, sizeof imageSize, 1, file);
 
   bits32 hResolution = PIX_PER_METRE;
   fwrite (&hResolution, sizeof hResolution, 1, file);
 
   bits32 vResolution = PIX_PER_METRE;
   fwrite (&vResolution, sizeof vResolution, 1, file);
 
   bits32 numColors = NUM_COLORS;
   fwrite (&numColors, sizeof numColors, 1, file);
 
   bits32 importantColors = NUM_COLORS;
   fwrite (&importantColors, sizeof importantColors, 1, file);
 
}