#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_decimal.h"

void s21_get_sign_get_scale_decimal(s21_decimal *value, int *sign, int *scale) {
  bit3 value_bit = {0};
  value_bit.bit = value->bits[3];
  *sign = value_bit.parts.sign;
  *scale = value_bit.parts.scale;
}

int s21_check_big_decimal(s21_big_decimal *value) {
  int check = 1;
  for (int i = count_element_in_decimal;
       i < count_element_in_big_decimal && check; i++) {
    if (value->bits[i] != 0) check = 0;
  }
  return check;
}

void s21_decimal_to_big_decimal(s21_decimal *num_dec,
                                s21_big_decimal *num_big_dec) {
  for (int i = 0; i < count_element_in_decimal; i++) {
    num_big_dec->bits[i] = num_dec->bits[i];
  }
}

void s21_big_decimal_to_decimal(s21_decimal *dec, s21_big_decimal *big_dec) {
  for (int i = 0; i < count_element_in_decimal; i++) {
    dec->bits[i] = big_dec->bits[i];
  }
}

void s21_add_number_big_decimal(int value_1, int value_2, int *result,
                                int *temp) {
  for (int i = 0; i < count_bits_in_int; i++) {
    int num_1 = (value_1 >> i) & 1;
    int num_2 = (value_2 >> i) & 1;
    int res = num_1 + num_2 + *temp;
    if (res == 0) {
      *result &= ~(1 << i);
      *temp = 0;
    } else if (res == 1) {
      *result |= (1 << i);
      *temp = 0;
    } else if (res == 2) {
      *result &= ~(1 << i);
      *temp = 1;
    } else if (res == 3) {
      *result |= (1 << i);
      *temp = 1;
    }
  }
}

void s21_add_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                         s21_big_decimal *result) {
  int temp = 0;
  for (int i = 0; i < count_element_in_big_decimal; i++) {
    s21_add_number_big_decimal(value_1.bits[i], value_2.bits[i],
                               &result->bits[i], &temp);
  }
}

void s21_shift_left_by_count(s21_big_decimal value, s21_big_decimal *result,
                             int count) {
  *result = (s21_big_decimal){0};
  for (int i = 0; i < count_element_in_big_decimal - 1; i++) {
    for (int j = 0; j < count_bits_in_int; j++) {
      int bit = (value.bits[i] >> j) & 1;
      if (j < count_bits_in_int - count) {
        result->bits[i] |= (bit << (j + count));
      } else {
        result->bits[i + 1] |= (bit << (j + count - count_bits_in_int));
      }
    }
  }
}

void s21_mult_by_10_n_big_decimal(s21_big_decimal *value, int n) {
  for (int i = 0; i < n; i++) {
    s21_big_decimal num = {0};
    s21_big_decimal num2 = {0};
    s21_shift_left_by_count(*value, &num, 3);
    s21_shift_left_by_count(*value, &num2, 1);
    s21_add_big_decimal(num, num2, value);
  }
}

void s21_sub_number_big_decimal(int value_1, int value_2, int *result,
                                int *temp) {
  for (int i = 0; i < count_bits_in_int; i++) {
    int num_1 = (value_1 >> i) & 1;
    int num_2 = (value_2 >> i) & 1;
    int res = num_1 - num_2 - *temp;
    if (res == 1) {
      *result |= (1 << i);
      *temp = 0;
    } else if (res == 0) {
      *result &= ~(1 << i);
      *temp = 0;
    } else if (res == -1) {
      *result |= (1 << i);
      *temp = 1;
    } else if (res == -2) {
      *result &= ~(1 << i);
      *temp = 1;
    }
  }
}

void s21_sub_big_decimal(s21_big_decimal value_1, s21_big_decimal value_2,
                         s21_big_decimal *result) {
  int temp = 0;
  for (int i = 0; i < count_element_in_big_decimal; i++) {
    s21_sub_number_big_decimal(value_1.bits[i], value_2.bits[i],
                               &result->bits[i], &temp);
  }
}

void s21_set_sign_scale(s21_decimal *value, int sign_res, int max_scale) {
  bit3 *parting = (bit3 *)&value->bits[3];
  parting->parts.scale = max_scale;
  parting->parts.sign = sign_res;
}

void s21_reminder_creator(s21_big_decimal *value, int num) {
  s21_big_decimal temp = {0};
  for (int i = 0; i < count_element_in_big_decimal; i++) {
    for (int j = 0; j < count_bits_in_int; j++) {
      int bit = (value->bits[i] >> j) & 1;
      if (j < count_bits_in_int - 1) {
        temp.bits[i] |= (bit << (j + 1));
      } else {
        temp.bits[i + 1] |= (bit << (j + 1 - count_bits_in_int));
      }
    }
  }
  if (num == 1) {
    temp.bits[0] |= 1;
  }
  *value = temp;
}

void s21_div_big_decimal(s21_big_decimal *dividend, s21_big_decimal *divisor,
                         s21_big_decimal *quotient,
                         s21_big_decimal *remainder) {
  s21_big_decimal remainder_temp = {0};
  for (int i = count_element_in_big_decimal - 1; i >= 0; i--) {
    for (int j = count_bits_in_int - 1; j >= 0; j--) {
      int bit = (dividend->bits[i] >> j) & 1;
      s21_reminder_creator(remainder, bit);
      if (!s21_is_less_big_decimal(remainder, divisor)) {
        s21_sub_big_decimal(*remainder, *divisor, &remainder_temp);
        *remainder = remainder_temp;
        remainder_temp = (s21_big_decimal){0};
        quotient->bits[i] |= (1 << j);
      }
    }
  }
}

void s21_bank_round(s21_big_decimal *value_result_big, int *max_scale) {
  s21_big_decimal del_10 = {.bits = {[0] = 10}};
  s21_big_decimal ostatok = {0};
  while ((!s21_check_big_decimal(value_result_big) && *max_scale) ||
         (*max_scale > maximum_scale)) {
    (*max_scale)--;
    s21_big_decimal temp = {0};
    ostatok = (s21_big_decimal){0};
    s21_div_big_decimal(value_result_big, &del_10, &temp, &ostatok);
    *value_result_big = temp;
  }
  if (ostatok.bits[0] > round_value || (ostatok.bits[0] == round_value &&
                                        (value_result_big->bits[0] & 1) == 1)) {
    s21_big_decimal value_one = {.bits = {[0] = 1}};
    s21_big_decimal temp = {0};
    s21_add_big_decimal(*value_result_big, value_one, &temp);
    *value_result_big = temp;
  }
}

void s21_mul_big_decimal(s21_big_decimal small, s21_big_decimal big,
                         s21_big_decimal *result) {
  *result = (s21_big_decimal){0};
  s21_big_decimal temp = {0};
  for (int i = 0; i < count_element_in_big_decimal; i++) {
    for (int j = 0; j < count_bits_in_int; j++) {
      if ((small.bits[i] >> j) & 1) {
        s21_big_decimal value_shift = {0};
        if (i != 0) {
          s21_shift_left_by_count(big, &value_shift, 32);
          for (int k = 2; k <= i; k++) {
            s21_shift_left_by_count(value_shift, &temp, 32);
            value_shift = temp;
          }
          temp = (s21_big_decimal){0};
          s21_shift_left_by_count(value_shift, &temp, j);
          value_shift = temp;
        } else {
          s21_shift_left_by_count(big, &value_shift, j);
        }
        temp = (s21_big_decimal){0};
        s21_add_big_decimal(*result, value_shift, &temp);
        *result = temp;
      }
    }
  }
}

void clearData(s21_decimal *number) {
  s21_decimal empty_value = {{0, 0, 0, 0}};
  *number = empty_value;
}

int s21_check_decimal(s21_decimal *value) {
  int flag = 0;
  if (value != NULL) {
    bit3 value_bit = {0};
    value_bit.bit = value->bits[3];
    if (value_bit.parts.empty1 || value_bit.parts.empty2 ||
        value_bit.parts.scale > maximum_scale)
      flag = 1;
  } else {
    flag = 1;
  }
  return flag;
}