#include <math.h>
#include <pixint.h>
#include <stdio.h>
#include <stdlib.h>

enum FLAGS {
	T_FLAG = 1 << 0,
	n_FLAG = 1 << 1,
	s_FLAG = 1 << 2,
	h_FLAG = 1 << 3
};

void NewNumberedLine(FILE * file, u32 * line_count) {
	char nc = fgetc(file);
	ungetc(nc, file);
	if (nc != EOF) {
		printf("     %d\t", (*line_count)++);
	}
}

int main(int argc, char ** argv) {
	u8 file_arg = 0;
	u8 flags = 0;
	for (u8 i = 1; i < argc; i++ ) {
		if (argv[i][0] == '-') {
			if (argv[i][1] == 'T') {
				flags |= T_FLAG;
			} else if (argv[i][1] == 'n') {
				flags |= n_FLAG;
			} else if (argv[i][1] == 's') {
				flags |= s_FLAG;
			} else if (argv[i][1] == 'h') {
				flags |= h_FLAG;
			} else {
				fprintf(stderr, "Flag -%c does not exist.\n", argv[i][1]);
				return 3;
			}
		}
	}
	if ((flags & h_FLAG) > 0) {
		printf("\nmy_cat by pixlark\n"
			   "\n-T: Print tab characters as ^I.\n"
			   "-n: Print line numbers."
			   "\n-s: Compress repeated blank lines."
			   "\n-h: Print this help menu.\n\n");
		return 0;
	}
	for (u8 i = 1; i < argc; i++) {
		if (*argv[i] != '-') {
			FILE * file = fopen(argv[i], "r");
			u32 line_count = 1;
			u32 con_newl = 0;
			if (file == 0) {
				fprintf(stderr, "File \"%s\" not found.\n", argv[i]);
				return 2;
			}
			if ((flags & n_FLAG) > 0) NewNumberedLine(file, &line_count);
			for (;;) {
				char c = fgetc(file);
				if (c == EOF) break;
				if (c != '\n') con_newl = 0;
				
				if (c == '\t' && (flags & T_FLAG) > 0) {
					printf("^I");
				} else if (c == '\n') {
					u8 print_newline = 0xFF;
					con_newl++;
					if ((flags & s_FLAG) > 0) {
						if (con_newl < 3) print_newline = 0xFF;
						else print_newline = 0x00;
					}
					if (print_newline) {
						printf("%c", c);
						if ((flags & n_FLAG) > 0) {
							NewNumberedLine(file, &line_count);
						}
					}
				} else {
					printf("%c", c);
				}
			}
			file_arg = 0xFF;
			fclose(file);
		}
	}
	if (file_arg == 0) {
		fprintf(stderr, "No files specified\n");
		return 1;
	}
}
