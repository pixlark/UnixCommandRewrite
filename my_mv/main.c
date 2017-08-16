/** COPYRIGHT (C) 2017
 ** https://pixlark.github.io/
 *
 ** main.c
 * 
 * Simple implementation of the mv function
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <pixint.h>

#define READ_BUFFER_SIZE 128

int main(int argc, char ** argv)
{
	if (argc < 3) {
		fprintf(stderr, "Usage: mv source destination\n");
		return 1;
	}
	
	FILE * read_file = fopen(argv[1], "r");
	FILE * write_file = fopen(argv[2], "w");

	char read_buffer[READ_BUFFER_SIZE];
	u8 end_of_file = 0;
	while (!end_of_file) {
		u32 chars_written = 0;
		for (int i = 0; i < READ_BUFFER_SIZE; i++, chars_written++) {
			char rc = fgetc(read_file);
			if (rc == EOF) {
				end_of_file = ~0;
				break;
			}
			read_buffer[i] = rc;
		}
		fwrite(read_buffer, chars_written, sizeof(char), write_file);
	}

	unlink(argv[1]);
	
	return 0;
}
