#ifndef AUXILIARY_H
#define AUXILIARY_H
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

enum values {
  plus,
  minus,
  byte_to_bits = 8,
  count_bits_in_int = sizeof(int) * byte_to_bits,
  count_element_in_decimal = 3,
  count_element_in_big_decimal = 8,
  sign_bits = 31,
  maximum_scale = 28,
  round_value = 5
};

typedef struct {
  int bits[4];
} s21_decimal;

typedef struct {
  int bits[8];
} s21_big_decimal;

typedef union decimal_bit3 {
  int bit;
  struct {
    uint32_t empty1 : 16;
    uint32_t scale : 8;
    uint32_t empty2 : 7;
    uint32_t sign : 1;
  } parts;
} bit3;
void setDecimalSign(s21_decimal *decimal, int sign);

void s21_get_sign_get_scale_decimal(s21_decimal *value, int *sign, int *scale);

void s21_get_sign_get_scale_two_decimal(s21_decimal *value_1,
                                        s21_decimal *value_2, int *sign_value_1,
                                        int *sign_value_2, int *scale_value_1,
                                        int *scale_value_2);

int s21_is_less_big_decimal(s21_big_decimal *, s21_big_decimal *);
int s21_is_less(s21_decimal, s21_decimal);
int s21_is_greater_big_decimal(s21_big_decimal *, s21_big_decimal *);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_equal_big_decimal(s21_big_decimal *, s21_big_decimal *);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void s21_div_after_check(s21_decimal value_1, s21_decimal value_2,
                         s21_decimal *result, int *flag);

int s21_dec_mod_10(s21_decimal src);

void s21_decimal_to_big_decimal(s21_decimal *, s21_big_decimal *);
void s21_big_decimal_to_decimal(s21_decimal *, s21_big_decimal *);
int s21_check_big_decimal(s21_big_decimal *);
int s21_check_decimal(s21_decimal *);
int zero_check(s21_decimal is_zero);
void s21_add_big_decimal(s21_big_decimal, s21_big_decimal, s21_big_decimal *);
void s21_add_number_big_decimal(int, int, int *, int *);
void s21_mult_by_10_n_big_decimal(s21_big_decimal *, int);
void s21_shift_left_by_count(s21_big_decimal value, s21_big_decimal *result,
                             int count);

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void s21_mul_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                         s21_big_decimal *result);

void s21_get_sign_get_scale_decimal(s21_decimal *, int *, int *);
void s21_sub_number_big_decimal(int value_1, int value_2, int *result,
                                int *temp);
void s21_sub_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                         s21_big_decimal *result);
void s21_set_sign_scale(s21_decimal *value, int sign, int scale);
void s21_div_big_decimal(s21_big_decimal *dividend, s21_big_decimal *divisor,
                         s21_big_decimal *quotient, s21_big_decimal *remainder);
void s21_reminder_creator(s21_big_decimal *value, int num);
void s21_bank_round(s21_big_decimal *value_result_big, int *max_scale);

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
void clearData(s21_decimal *number);

int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);
#endif