#include <stdio.h>
#include <stdlib.h>

/* Performs 1 byte XOR search for ASCII string.

To compile, use `gcc -o xorascii xorascii.c` or your other favorite compiler with appropriate args.

Frazier Baker, 2019 February 16.
Published under MIT license (following):

Copyright 2019 Frazier Baker

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

int main(int argc, char** argv) {
	if(argc < 2) {
		printf("Usage: %s file\n", argv[0]);
		return 1;
	}
	unsigned char* b = malloc(1);
	for(unsigned char c = (unsigned char)0; c <= 0xFF; c++) {
		short good = 1; // short in place of bool because C, tracks whether key was good
		FILE* fptr = fopen(argv[1], "rb"); // XOR-encoded file
		unsigned char* outputf = malloc(100); // Output filename
		sprintf(outputf, "%s-XOR.0x%02x",argv[1],c);
		FILE* outptr = fopen(outputf, "wb");
		free(outputf); // FREE outputf
		int ascii = 0; // Counts valid (printable) ascii
		int total = 0; // Counts total bytes
		while(!feof(fptr)) {
			fread(b, 1, 1, fptr);
			unsigned char x = (unsigned char)(*b ^ c);
			fwrite(&x, 1, 1, outptr);
			if((x < 32 && x != '\n' && x != '\r') || x > 127) {
				if(good) { printf("Bad XOR %02x",c); }
				good = 0;
				total++;
			} else {
				ascii++;
				total++;
			}
		}
		if(good) {
			printf("***            Good XOR %02x            ***\n",c); // Yay!
		}
		else {
			printf(" -- Valid Bytes (ASCII): %i / %i\n", ascii, total); // Maybe nearly all ASCII?
		}
		fclose(fptr);
		fclose(outptr);
		if(c == 0xFF) {
			break; // Avoid infinite loop with char (wraps at 255)
		}
	}
	*b = (char)0x00; // Zero out data
	free(b); // FREE b
	return 0;
}
