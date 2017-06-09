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
#include <unistd.h>

int main() {
	struct winsize ws;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
	printf("cols: %d\nrows: %d\n", ws.ws_col, ws.ws_row);
	return 0;
}
