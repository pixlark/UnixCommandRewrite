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
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

struct termios enable_char_input() {
	struct termios t_old;
	tcgetattr(STDIN_FILENO, &t_old);
	struct termios t_new = t_old;
	t_new.c_lflag &= ~(ICANON);
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
		char c = fgetc(file);
		if (c == '\n') break;
		buffer[buf_index] = c;
		buf_index++;
	}
	buffer[buf_index] = '\0';
	return buffer;
}

int main() {
	struct winsize ws = get_window_size();
	struct termios t_old = enable_char_input();
	FILE * read_file = fopen("test_file", "r");
	int tc;
	while ((tc = fgetc(read_file)) != EOF) {
		ungetc(tc, read_file);
		for (int i = 0; i < ws.ws_row - 1; i++) {
			printf("%s\n", read_line(read_file));
		}
		/*for (;;) {
			if (getchar() == ' ') break;
			else {
				printf("space to continue");
			}
			}*/
	}
	reset_termios(t_old);
	return 0;
}
