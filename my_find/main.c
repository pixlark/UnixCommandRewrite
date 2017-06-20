/** COPYRIGHT (C) 2017
 ** https://pixlark.github.io/
 *
 ** main.c
 * 
 * This file conatins a simplified implementation of the Unix command
 * "find".
 *
 */

#include <dirent.h>
#include <pixint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

void search_directory(char * search_dir_path, char * search_term) {
	DIR * search_dir = opendir(search_dir_path);
	struct dirent * search_ent;
	while ((search_ent = readdir(search_dir)) != 0) {
		if (strcmp(search_ent->d_name, search_term) == 0) {
			printf("Found at: %s/%s\n", search_dir_path, search_ent->d_name);
		}
		struct stat * ent_stat = malloc(sizeof(struct stat));
		stat(search_ent->d_name, ent_stat);
		if (S_ISDIR(dir_test_stat->st_mode)) {
			strcat()
			search_directory(strcat()
		}
		free(ent_stat);
	}
}

int main(int argc, char ** argv) {
	if (argc < 3) {
		fprintf(stderr, "Too few arguments.\n");
		exit(0);
	}
	argv++;
	char * starting_directory = *argv;
	argv++;
	char * search_term = *argv;

	// Get starting directory
	struct stat * dir_test_stat = malloc(sizeof(struct stat));
	stat(starting_directory, dir_test_stat);
	if (!S_ISDIR(dir_test_stat->st_mode)) {
		fprintf(stderr, "ERROR: Search directory doesn't exist.\n");
		exit(0);
	}
	free(dir_test_stat);

	// Recursively search directories
	search_directory(starting_directory, search_term);
	
	return 0;
}
