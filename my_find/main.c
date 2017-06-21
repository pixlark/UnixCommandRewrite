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
#include <errno.h>
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
		// Exclude . and .. directories
		if (strcmp(search_ent->d_name, ".") == 0 || strcmp(search_ent->d_name, "..") == 0) {
			continue;
		}
		// Get path in context
		char * context_path = malloc((sizeof(char) * strlen(search_dir_path) + 2) +
									 (sizeof(char) * strlen(search_ent->d_name) + 1));
		strcpy(context_path, search_dir_path);
		strcat(context_path, "/");
		strcat(context_path, search_ent->d_name);
		// Check for search term
		if (strcmp(search_ent->d_name, search_term) == 0) {
			printf("Found at: %s\n", context_path);
		}
		// Recurse through if directory
		struct stat * ent_stat = malloc(sizeof(struct stat));
		stat(context_path, ent_stat);
		if (S_ISDIR(ent_stat->st_mode)) {
			search_directory(context_path, search_term);
		}
		free(context_path);
		free(ent_stat);
	}
	free(search_ent);
	free(search_dir);
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
