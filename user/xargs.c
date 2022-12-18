#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
	

	if (argc < 2) {
		fprintf(2, "xargs need at least 2 params.\n");
		exit(1);
	}

	if (argc > MAXARG) {
		fprintf(2, "Too many args for xargs.\n");
		exit(1);
	}

	char* xarg_params[MAXARG];
	int xarg_params_idx;

	// Copy from the front;
	for (xarg_params_idx = 0; xarg_params_idx < argc - 1; xarg_params_idx ++) {
		xarg_params[xarg_params_idx] = argv[xarg_params_idx + 1];
	}

	char buffer[512];
	int index = xarg_params_idx;
	int buffer_index = 0;
	int lst = 0;
	int is_line_break = 0;
	while (read(0, buffer + buffer_index, sizeof(char))) {
		
		if (buffer[buffer_index] == '\n') {
			is_line_break = 1;
		}

		if (buffer[buffer_index] == ' ' || is_line_break) {
			buffer[buffer_index] = 0;
			xarg_params[index] = &buffer[lst];
			index ++;
			lst = buffer_index + 1;
			if (is_line_break) {
				lst = 0;
				buffer_index = 0;
			}
		}

		if (is_line_break) {
			is_line_break = 0;
			
			
			if (fork() == 0) {
				// child process
				exec(xarg_params[0], xarg_params);
			} else {
				wait(0);
				
				// clean the unnecessary xarg_params per line. 
				while (index >= argc - 1) {
					xarg_params[index --] = 0;
				}
				index ++;
			}
		} else {
			buffer_index ++;
		}
	}
	exit(0);
}