#include <dirent.h>
#include <pixint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define FRMT_BOLD "\e[1m"
#define FRMT_RESET "\e[0m"

#define CLR_RED     "\x1b[31m"
#define CLR_GREEN   "\x1b[32m"
#define CLR_YELLOW  "\x1b[33m"
#define CLR_BLUE    "\x1b[34m"
#define CLR_MAGENTA "\x1b[35m"
#define CLR_CYAN    "\x1b[36m"
#define CLR_RESET   "\x1b[0m"

enum flag_names {
	a_FLAG = 1 << 0,
	l_FLAG = 1 << 1,
	h_FLAG = 1 << 2
};

void print_ent(struct dirent * ent) {
	struct stat * ent_stat = malloc(sizeof(struct stat));
	stat(ent->d_name, ent_stat);
	char * frmt = FRMT_RESET;
	char * clr = CLR_RESET;
	if (ent_stat->st_mode & S_IXUSR) {
		frmt = FRMT_BOLD;
		clr = CLR_GREEN;
	}
	if (ent_stat->st_mode & S_IFDIR) {
		frmt = FRMT_BOLD;
		clr = CLR_BLUE;
	}
	printf("%s%s%s ",
		   frmt, clr,
		   ent->d_name);
}

void print_ent_long(struct dirent * ent) {
	struct stat * ent_stat = malloc(sizeof(struct stat));
	stat(ent->d_name, ent_stat);

	// File size
	printf(FRMT_RESET CLR_RESET "% 10ld ", ent_stat->st_size);
	
	// Date and Time
	char date_buffer[24];
	struct tm tm_time = *localtime(&ent_stat->st_mtime);
	strftime(date_buffer, sizeof(date_buffer), "%d-%m-%Y %H:%M", &tm_time);
	printf(FRMT_RESET CLR_RESET "%s ", date_buffer);
}

int main(int argc, char ** argv) {
	u8 flags = 0;
	for (u8 i = 0; i < argc; i++) {
		if (strcmp(argv[i], "-a") == 0) {
			flags |= a_FLAG;
		} else if (strcmp(argv[i], "-h") == 0) {
			flags |= h_FLAG;
		} else if (strcmp(argv[i], "-l") == 0) {
			flags |= l_FLAG;
		}
	}

	if (flags & h_FLAG) {
		printf("\nmy_ls by pixlark\n\n");
		printf("-a: Display hidden items.\n");
		printf("-l: Display extra information.\n");
		printf("-h: Display this help menu.\n\n");
		exit(0);
	}

	#define CWD_PATH_SIZE 1024
	char cwd_path[CWD_PATH_SIZE];
	if(getcwd(cwd_path, CWD_PATH_SIZE) == 0) {
		fprintf(stderr, "ERROR: Invalid working directory\n");
		exit(0);
	}
	DIR * cwd_dir;
	struct dirent * cwd_ent;
	if ((cwd_dir = opendir(cwd_path)) == 0) {
		fprintf(stderr, "ERROR: Invalid DIR*\n");
		exit(0);
	}

	while ((cwd_ent = readdir(cwd_dir)) != 0) {
		u8 to_print = 0x00;
		if (flags & a_FLAG) {
			to_print = 0xFF;
		} else {
			if (strcmp(cwd_ent->d_name, ".") != 0 &&
				strcmp(cwd_ent->d_name, "..") != 0 &&
				cwd_ent->d_name[0] != '.') {
				to_print = 0xFF;
			}
		}
		if (to_print) {
			if (flags & l_FLAG) {
				print_ent_long(cwd_ent);
				print_ent(cwd_ent);
				printf("\n");
			} else {
				print_ent(cwd_ent);
			}
		}
	}

	if ((flags & l_FLAG) == 0) printf("\n");
	
	return 0;
}
