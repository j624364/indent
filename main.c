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

void set_indent_string(const char* source) {
	int origpos, newpos, length, is_escaped;

	if (indent_string != NULL) {
		free(indent_string);
	}

	origpos = 0;
	newpos = 0;
	is_escaped = 0;

	length = strlen(source);
	indent_string = (char*)calloc(length, sizeof(char));

	for (origpos = 0; origpos < length; origpos++) {
		if (is_escaped) {
			/* just add ones from c */
			switch (source[origpos]) {
				case 'a':
					indent_string[newpos++] = '\a';
					break;
				case 'b':
					indent_string[newpos++] = '\b';
					break;
				case 'f':
					indent_string[newpos++] = '\f';
					break;
				case 'n':
					indent_string[newpos++] = '\n';
					break;
				case 'r':
					indent_string[newpos++] = '\r';
					break;
				case 't':
					indent_string[newpos++] = '\t';
					break;
				case 'v':
					indent_string[newpos++] = '\v';
					break;
				case '\\':
					indent_string[newpos++] = '\\';
					break;
				case '\'':
					indent_string[newpos++] = '\'';
					break;
				case '"':
					indent_string[newpos++] = '"';
					break;
				case '?':
					indent_string[newpos++] = '?';
					break;
				default:
					indent_string[newpos++] = source[origpos];
					break;
			}
		}
		else if (source[origpos] == '\\') {
			is_escaped = 1;
		}
		else {
			indent_string[newpos++] = source[origpos];
		}
	}
}

void print_indent() {
	int i;

	for (i = 0; i < indent_level; i++) {
		printf("%s", indent_string);
	}
}

void set_default_indent() {
	set_indent_string(" ");
}

void print_help_message() {
	if (indent_string == NULL)
		set_default_indent();

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
	return strcmp(msg1, msg2) == 0;
}

int parse_args(int argc, char** argv) {
	int i, has_custom_indent;
	str2int_errno int_conv_res;

	has_custom_indent = 0;

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

			set_indent_string(argv[i]);
			has_custom_indent = 1;
		}
		else {
			print_help_message();
			return EXIT_FAILURE;
		}
	}

	if (!has_custom_indent)
		set_default_indent();

	return EXIT_SUCCESS;
}

int read_stdin() {
	bytes_read = read(STDIN_FILENO, stdin_buffer, STDIN_BUFFER_SIZE);

	return bytes_read > 0;
}

void clearup() {
	if (indent_string != NULL)
		free(indent_string);
}

int main(int argc, char** argv) {

	int res, i, is_newline;

	indent_string = NULL;

	/* parse the command line arguments */
	res = parse_args(argc, argv);
	if (res != EXIT_SUCCESS) {
		clearup();
		return res;
	}

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

	clearup();
	return EXIT_SUCCESS;
}

