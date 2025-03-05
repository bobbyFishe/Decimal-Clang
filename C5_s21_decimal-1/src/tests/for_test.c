#include "for_test.h"
#include <stdio.h>
#include <string.h>
#include "../s21_decimal.h"

void parse_binary_string_to_decimal(char *buffer, s21_decimal *decimal) {
    char *part = strtok(buffer, ",");
    for (int i = 0; i < 4 && part != NULL; i++) {
        decimal->bits[i] = binary_string_to_int(part);
        part = strtok(NULL, ",");
    }
}
int binary_string_to_int(const char *bin_str) {
    int result = 0;
    for (int i = 0; i < 32; i++) {
        result = (result << 1) | (bin_str[i] - '0');
    }
    return result;
}

void int_to_binary_string(int num, char *buffer) {
    for (int i = 0; i < 32; i++) {
        buffer[i] = ((num >> (31 - i)) & 1) ? '1' : '0';
    }
    buffer[32] = '\0';
}

void decimal_to_binary_string(int bits[4], char *output) {
    char temp[33];
    output[0] = '\0';

    for (int i = 0; i < 4; i++) {
        int_to_binary_string(bits[i], temp);
        if (i > 0) {
            snprintf(output + strlen(output), 2, ",");
        }
        snprintf(output + strlen(output), 33, "%s", temp);
    }
}