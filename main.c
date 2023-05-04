#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "string_to_int.h"

#define BUFFER_SIZE 1024

int indent_level = 4;
char* indent_string;
char file_buffer[BUFFER_SIZE];
int bytes_read = 0;
int has_printed_file = 0;

#ifdef _DEBUG
#include <assert.h>
unsigned int allocated = 0;
#endif

void* ccalloc(size_t num, size_t size) {
	void* ptr = calloc(num, size);

	if (ptr == NULL) {
		printf("Could not allocate memory!");
	}

#ifdef _DEBUG
	allocated++;
#endif

	return ptr;
}

void cfree(void* ptr) {
	if (ptr == NULL)
		return;

	free(ptr);

#ifdef _DEBUG
	allocated--;
#endif
}

void set_indent_string(const char* source) {
	int origpos, newpos, length, is_escaped;

	if (indent_string != NULL) {
		free(indent_string);
	}

	origpos = 0;
	newpos = 0;
	is_escaped = 0;

	length = strlen(source);
	indent_string = (char*)ccalloc(length, sizeof(char));

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

			is_escaped = 0;
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
	print_indent(); printf("Anything other option will print this message and exit the program.\n");
	print_indent(); printf("If a file can not be found it will be assumed to be an option.\n");
	printf("\n");
}

int streq(const char* msg1, const char* msg2) {
	return strcmp(msg1, msg2) == 0;
}

int file_exists(const char* path) {
	struct stat buffer;
	return (stat (path, &buffer) == 0);
}

int print_file(const char* path) {
	FILE* file;
	int i, is_newline, bytes_read;

	has_printed_file = 1;

	if (indent_string == NULL)
		set_default_indent();

	is_newline = 1;

	file = fopen(path, "r");
	if (!file)
		return EXIT_FAILURE;

	while ((bytes_read = fread(file_buffer, 1, BUFFER_SIZE, file)) > 0) {
		if (ferror(file)) {
			fclose(file);
			return EXIT_SUCCESS;
		}

		for (i = 0; i < bytes_read; i++) {
			if (file_buffer[i] == '\n') {
				putchar('\n');
				is_newline = 1;
			}
			else if (file_buffer[i] == '\0' || file_buffer[i] == EOF) {
				/* do nothing */
			}
			else {
				if (is_newline) {
					print_indent();
					is_newline = 0;
				}

				putchar(file_buffer[i]);
			}
		}
	}

	return EXIT_SUCCESS;
}

int parse_args(int argc, char** argv) {
	int i, has_custom_indent, res;
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
			/* assume a file */
			if (file_exists(argv[i])) {
				res = print_file(argv[i]);

				if (res != EXIT_SUCCESS)
					return res;
			}
			else {
				print_help_message();
				return EXIT_FAILURE;
			}
		}
	}

	if (!has_custom_indent)
		set_default_indent();

	return EXIT_SUCCESS;
}

void clearup() {
	if (indent_string != NULL)
		cfree(indent_string);

#if _DEBUG
	assert(allocated == 0);
#endif
}

int main(int argc, char** argv) {

	int res;

	indent_string = NULL;

	/* parse the command line arguments */
	res = parse_args(argc, argv);
	if (res != EXIT_SUCCESS) {
		clearup();
		return res;
	}

	if (!has_printed_file) {
		res = print_file("/dev/stdin");
		if (res != EXIT_SUCCESS) {
			clearup();
			return res;
		}
	}

	clearup();
	return EXIT_SUCCESS;
}

