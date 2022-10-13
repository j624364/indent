#ifndef INDENT_STRING_TO_INT
#define INDENT_STRING_TO_INT

typedef enum {
    STR2INT_SUCCESS,
    STR2INT_OVERFLOW,
    STR2INT_UNDERFLOW,
    STR2INT_INCONVERTIBLE
} str2int_errno;

str2int_errno str2int(int* out, char* s, int base);

#endif
