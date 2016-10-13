// Copyright (c) Richard Rohac & Filip Stollar, 2016, All rights reserved.

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <libgen.h>
#include <string.h>

typedef unsigned char byte;
#define RGB2COLOR(r, g, b) ((((r>>3)<<11) | ((g>>2)<<5) | (b>>3)))

char* itoa(int val, int base){
  static char buf[32] = {0};
  int i = 30;

  for(; val && i ; --i, val /= base) {
    buf[i] = "0123456789abcdef"[val % base];
  }
  return &buf[i+1];
}

byte readByte(FILE *pInput, FILE *pOutput) {
    byte output;
    if(!fread(&output, 1, 1, pInput)) {
        printf("Expected to read one more byte");
        fclose(pOutput);
        fclose(pInput);
        exit(1);
    }
    return output;
} 

int main(int argc, char **argv) {
    char *pszInputPath;
    char szExt[16];

	if(argc != 2) {
		printf("Define input image please\n");
		return 1;
	}

    pszInputPath = argv[1];

	FILE *pInput = fopen(pszInputPath, "rb");
	if(!pInput) {
		printf("Cannot open input file %s\n", pszInputPath);
		return 1;
	}

    // file name is of format path/name_WIDTH.rgb
    uint32_t width;
    char *pDirectory = dirname(strdup(pszInputPath));
    char *pFilename = basename(strdup(pszInputPath));

    // get last _ 
    char *pToken = strtok(strdup(pFilename), "_");
    while(1) {
        char *pTemp = strtok(NULL, "_");
        if(!pTemp)
            break;

        pToken = pTemp;
    }

    if(sscanf(pToken, "%d%s",  &width, szExt) != 2) {
        printf("File name should be in the form name_WIDTH.rgb!\n");
        fclose(pInput);
        return 1;
    }

    // calculate height based on the width and file size
    uint32_t height;
    fseek(pInput, 0, SEEK_END); 
    uint32_t fileSize = ftell(pInput);
    height = (fileSize / 3) / width;

    printf("Creating .img file from: %s with width %d and height %d\n", pFilename, width, height);

    fseek(pInput, 0, SEEK_SET); // rewind back

    char szOutputPath[256];
    strcpy(szOutputPath, pDirectory);
    strcat(szOutputPath, "/");
    strncat(szOutputPath, pFilename, strlen(pFilename) - 5 - strlen(itoa(width,10)));
    strcat(szOutputPath, ".img");
	FILE *pOutput = fopen(szOutputPath, "wb");
	if(!pOutput) {
		printf("Cannot open output file");
		return 1;
	}

    // write width
    if(!fwrite(&width, sizeof(uint32_t), 1, pOutput)) {
        printf("Cannot write width into output file");
        return 1;
    }

    // write height
    if(!fwrite(&height, sizeof(uint32_t), 1, pOutput)) {
        printf("Cannot write height into output file");
        return 1;
    }

    // make sure we are 32byte aligned, so write another 24 bytes
    unsigned char empty[24];
    memset(empty, 0, sizeof(char)*24);

     // write height
    if(!fwrite(&empty, sizeof(char)*24, 1, pOutput)) {
        printf("Cannot write dummy into output file");
        return 1;
    }



    // read in the file
	for(uint32_t i = 0; i < height; i++) {
		for(uint32_t j = 0; j < width; j++) {
            uint16_t col16 = 0;
            uint32_t r, g, b;

			r = readByte(pInput, pOutput);
            g = readByte(pInput, pOutput);
            b = readByte(pInput, pOutput);

            col16 = RGB2COLOR(r, g, b);
            if(!fwrite(&col16, sizeof(uint16_t), 1, pOutput)) {
                printf("Cannot write 16bit value into output");
                fclose(pOutput);
                fclose(pInput);
                return 1;
            }
		}
	}

	fclose(pOutput);
	fclose(pInput);

	return 0;
}
