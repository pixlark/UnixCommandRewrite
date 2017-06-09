#include <pixint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FRMT_BOLD "\e[1m"
#define FRMT_RESET "\e[0m"

#define CLR_RED     "\x1b[31m"
#define CLR_GREEN   "\x1b[32m"
#define CLR_YELLOW  "\x1b[33m"
#define CLR_BLUE    "\x1b[34m"
#define CLR_MAGENTA "\x1b[35m"
#define CLR_CYAN    "\x1b[36m"
#define CLR_RESET   "\x1b[0m"

typedef struct QNode {
	struct QNode * next;
	u16 data;
} QNode;

typedef struct {
	QNode * front;
	QNode * rear;
} Queue;

u16 QDequeue(Queue * queue) {
	if (queue->front == 0) {
		return 0;
	}
	u16 data = queue->front->data;
	QNode * to_free = queue->front;
	queue->front = queue->front->next;
	free(to_free);
	return data;
}

void QEnqueue(Queue * queue, u16 data) {
	QNode * new_node = malloc(sizeof(QNode));
	new_node->next = 0;
	new_node->data = data;
	if (queue->rear == 0) {
		queue->front = new_node;
		queue->rear = new_node;
	} else {
		queue->rear->next = new_node;
		queue->rear = new_node;
	}
}

int main (int argc, char ** argv) {
	if (argc <= 2) {
		fprintf(stderr, "Incorrect number of arguments.\n");
		exit(0);
	}
	argv++;
	char * search_term = *argv;

	for (int i = 0; i < argc-2;i++) {
		argv++;
		char * file_name = *argv;
		FILE * search_file = fopen(file_name, "r");
		printf(FRMT_BOLD CLR_GREEN "----%s----\n", file_name);
		u16 term_size = strlen(search_term);
		u16 current_line = 1;
		for (;;) {
			Queue * term_queue = malloc(sizeof(Queue));
			term_queue->front = 0;
			term_queue->rear = 0;
			u16 line_begin_offset = 0;
			u16 lterm_offset = 0;
			u8 term_this_line = 0x00;
			u8 end_of_file = 0x00;
			for (;;) {
				/* Move through until a newline is found, and push any
				   columns where the search term is detected onto a
				   queue. */
				char lc = fgetc(search_file);
				if (lc == EOF) {
					end_of_file = 0xFF;
					break;
				} else if (lc == '\n') {
					current_line++;
					break;
				} else if (lc == search_term[lterm_offset]) {
					lterm_offset++;
				} else {
					lterm_offset = 0;
				}
				if (lterm_offset == term_size) {
					term_this_line = 0xFF;
					QEnqueue(term_queue, line_begin_offset - (term_size - 1));
					lterm_offset = 0;
				}
				line_begin_offset++;
			}
			if (term_this_line) {
				/* If a term was found on this line, print the line, with
				   the characters red on every column where a term was
				   detected. */
				fseek(search_file, -(line_begin_offset + 1), SEEK_CUR);
				u16 next_term_column = QDequeue(term_queue);
				u16 print_offset = 0;
				for (;;) {
					char cc = fgetc(search_file);
					if (cc == '\n' || cc == EOF) break;
					if (print_offset == next_term_column) {
						char pc = cc;
						for(int i = 0; i < term_size; i++) {
							if (i > 0) pc = fgetc(search_file);
							printf(FRMT_BOLD CLR_RED "%c", pc);
						}
						next_term_column = QDequeue(term_queue);
					} else {
						printf(FRMT_RESET CLR_RESET "%c", cc);
					}
					print_offset++;
				}
				printf("\n");
			}
			if (end_of_file) {
				free(term_queue);
				break;
			}
		}
	}
	return 0;
}
