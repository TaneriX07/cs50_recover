#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int BLOCK_SIZE = 512;
  char *path = "card.raw";
  unsigned char buffer[BLOCK_SIZE];
  char filename[8];
  int jpeg_count = 0;
  FILE *img = NULL;

  // Open memory card
  FILE *inFile = fopen(path, "r");
  if (!inFile)
  {
    printf("Invalid file!\n");
    return 1;
  }

  // Repeat until end of card
  // Read 512 bytes into a buffer
  while (fread(buffer, 1, BLOCK_SIZE, inFile) > 0 )
  {
    // Check if it's the start of new JPEG
    if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
    {
      // If first JPEG
      if (jpeg_count == 0)
      {
        // Create a new file to write JPEG
        sprintf(filename, "%03i.jpg", jpeg_count);
        jpeg_count++;
        // Open the file
        img = fopen(filename, "w");
        printf("Open %s\n", filename);
      }
      else
      {
        // Close previous image
        fclose(img);
        printf("Close %s\n", filename);
        // Create a new file to write JPEG
        sprintf(filename, "%03i.jpg", jpeg_count);
        jpeg_count++;
        // Open the file
        img = fopen(filename, "w");
        printf("Open %s\n", filename);
      }
    }

    if (img != NULL)
    {
      printf("Writing %s\n", filename);
      fwrite(buffer, 1, BLOCK_SIZE, img);
    }
  }
  // Close remaining files
  fclose(img);
  fclose(inFile);
}
