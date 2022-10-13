#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "string_to_int.h"

#define STDIN_BUFFER_SIZE 1024

int indent_level = 4;
char* indent_string;
char stdin_buffer[STDIN_BUFFER_SIZE];
int bytes_read = 0;

void print_indent() {
	int i;

	for (i = 0; i < indent_level; i++) {
		printf("%s", indent_string);
	}
}

void print_help_message() {
	printf("\n");
	print_indent(); printf("Usage: indent [options]\n");
	print_indent(); printf("Reads from standard input and outputs to standard output with an indent.\n");
	printf("\n");
	print_indent(); printf("Options:\n");
	print_indent(); printf("--level, -l    \t Sets the number of characters to indent by. Default: 4\n");
	print_indent(); printf("--character, -c\t Sets the string to indent with: Default: ' '\n");
	printf("\n");
}

int streq(const char* msg1, const char* msg2) {
	return strcmp(msg1, msg2) != 0;
}

int parse_args(int argc, char** argv) {
	int i;
	str2int_errno int_conv_res;

	for (i = 1; i < argc; i++) {
		if (streq(argv[i], "--level") || streq(argv[i], "-l")) {
			i++;

			if (i >= argc) {
				print_help_message();
				return EXIT_FAILURE;
			}

			int_conv_res = str2int(&indent_level, argv[i], 10);
			if (int_conv_res != STR2INT_SUCCESS) {
				print_help_message();
				return EXIT_FAILURE;
			}
		}
		else if (streq(argv[i], "--character") || streq(argv[i], "-c")) {
			i++;

			if (i >= argc) {
				print_help_message();
				return EXIT_FAILURE;
			}

			indent_string = argv[i];
		}
		else {
			print_help_message();
			return EXIT_FAILURE;
		}
	}

	return EXIT_SUCCESS;
}

int read_stdin() {
	bytes_read = read(STDIN_FILENO, stdin_buffer, STDIN_BUFFER_SIZE);

	return bytes_read > 0;
}

int main(int argc, char** argv) {

	int res, i, is_newline;

	indent_string = " ";

	/* parse the command line arguments */
	res = parse_args(argc, argv);
	if (res != EXIT_SUCCESS)
		return res;

	is_newline = 1;

	while(read_stdin()) {
		for (i = 0; i < bytes_read; i++) {
			if (stdin_buffer[i] == '\n') {
				putchar('\n');
				is_newline = 1;
			}
			else if (stdin_buffer[i] == '\0' || stdin_buffer[i] == EOF) {
				/* do nothing */
			}
			else {
				if (is_newline) {
					print_indent();
					is_newline = 0;
				}

				putchar(stdin_buffer[i]);
			}
		}
	}	

	return EXIT_SUCCESS;
}

