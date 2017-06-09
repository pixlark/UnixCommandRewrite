/** COPYRIGHT (C) 2017
 ** https://pixlark.github.io/
 *
 ** main.c
 * 
 * This file holds a clone of the Unix command "more" which displays a
 * file one screen at a time for easy reading in terminals that lack a
 * scrolling feature.
 *
 */

#include <pixint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

struct termios enable_char_input() {
	struct termios t_old;
	tcgetattr(STDIN_FILENO, &t_old);
	struct termios t_new = t_old;
	t_new.c_lflag &= ~(ICANON);
	t_new.c_lflag &= ~(ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &t_new);
	return t_old;
}

void reset_termios(struct termios t_old) {
	tcsetattr(STDIN_FILENO, TCSANOW, &t_old);
}

struct winsize get_window_size() {
	struct winsize ws;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
	return ws;
}

char * read_line(FILE * file) {
	char * buffer = malloc(sizeof(char) * 256);
	u16 buf_index = 0;
	for (;;) {
		int c = fgetc(file);
		if (c == '\n') break;
		if (c == EOF) {
			buffer[0] = '\0';
			return buffer;
		}
		buffer[buf_index] = c;
		buf_index++;
	}
	buffer[buf_index] = '\n';
	buffer[buf_index+1] = '\0';
	return buffer;
}

int main(int argc, char ** argv) {
	if (argc < 2) {
		fprintf(stderr, "Too few arguments.");
		exit(1);
	}
	argv++;
	struct winsize ws = get_window_size();
	struct termios t_old = enable_char_input();
	FILE * read_file = fopen(*argv, "r");
	int tc;
	while ((tc = fgetc(read_file)) != EOF) {
		ungetc(tc, read_file);
		for (int i = 0; i < ws.ws_row - 1; i++) {
			char * next_line = read_line(read_file);
			printf("%s", next_line);
			free(next_line);
		}
		u8 instruction_given = 0;
		for (;;) {
			if (getchar() == ' ') break;
			else {
				if (!instruction_given) {
					printf("PRESS SPACE TO ADVANCE");
					instruction_given = ~0;
				}
			}
		}
		if (instruction_given) {
			printf("\033[2K\r"); // Erase current line
		}
	}
	reset_termios(t_old);
	return 0;
}
