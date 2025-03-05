#include <stdlib.h>

#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  clearData(result);
  int sign_1, sign_2, sign_res, scale_1, scale_2, sub_scale, max_scale,
      flag = 0;
  s21_big_decimal value_1_big = {0}, value_2_big = {0}, value_result_big = {0};
  s21_get_sign_get_scale_decimal(&value_1, &sign_1, &scale_1);
  s21_get_sign_get_scale_decimal(&value_2, &sign_2, &scale_2);
  s21_decimal_to_big_decimal(&value_1, &value_1_big);
  s21_decimal_to_big_decimal(&value_2, &value_2_big);
  max_scale = scale_1 > scale_2 ? scale_1 : scale_2;
  sub_scale = scale_1 - scale_2;
  if (sub_scale < 0) {
    s21_mult_by_10_n_big_decimal(&value_1_big, abs(sub_scale));
  } else if (sub_scale > 0) {
    s21_mult_by_10_n_big_decimal(&value_2_big, abs(sub_scale));
  }
  if (sign_1 == sign_2) {
    sign_res = sign_1;
    s21_add_big_decimal(value_1_big, value_2_big, &value_result_big);
  } else {
    if (s21_is_less_big_decimal(&value_1_big, &value_2_big)) {
      s21_sub_big_decimal(value_2_big, value_1_big, &value_result_big);
      sign_res = sign_1 < sign_2 ? minus : plus;
    } else if (s21_is_greater_big_decimal(&value_1_big, &value_2_big)) {
      s21_sub_big_decimal(value_1_big, value_2_big, &value_result_big);
      sign_res = sign_1 < sign_2 ? plus : minus;
    }
  }
  s21_bank_round(&value_result_big, &max_scale);
  if (s21_check_big_decimal(&value_result_big) == 1) {
    s21_big_decimal_to_decimal(result, &value_result_big);
    s21_set_sign_scale(result, sign_res, max_scale);
  } else {
    flag = sign_res == 0 ? 1 : 2;
  }
  return flag;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  clearData(result);
  s21_decimal temp = {0};
  s21_negate(value_2, &temp);
  return s21_add(value_1, temp, result);
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  clearData(result);
  int sign_1, sign_2, sign_res, scale_1, scale_2, two_scale, flag = 0;
  s21_big_decimal value_1_big = {0}, value_2_big = {0}, value_result_big = {0};
  s21_get_sign_get_scale_decimal(&value_1, &sign_1, &scale_1);
  s21_get_sign_get_scale_decimal(&value_2, &sign_2, &scale_2);
  s21_decimal_to_big_decimal(&value_1, &value_1_big);
  s21_decimal_to_big_decimal(&value_2, &value_2_big);
  two_scale = scale_1 + scale_2;
  sign_res = (sign_1 == sign_2) ? 0 : 1;
  int better = s21_is_less_big_decimal(&value_1_big, &value_2_big);
  (better) ? s21_mul_big_decimal(value_1_big, value_2_big, &value_result_big)
           : s21_mul_big_decimal(value_2_big, value_1_big, &value_result_big);
  s21_bank_round(&value_result_big, &two_scale);
  if (s21_check_big_decimal(&value_result_big) == 1) {
    s21_big_decimal_to_decimal(result, &value_result_big);
    s21_set_sign_scale(result, sign_res, two_scale);
  } else {
    flag = sign_res == 0 ? 1 : 2;
  }
  return flag;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  clearData(result);
  int flag = 0, ostatok = 0;
  if (s21_is_equal(value_2, (s21_decimal){0}))
    flag = 3;
  else if (s21_is_equal(value_1, (s21_decimal){0}))
    *result = (s21_decimal){0};
  else {
    s21_div_after_check(value_1, value_2, result, &flag);
  }
  return flag;
}

void s21_div_after_check(s21_decimal value_1, s21_decimal value_2,
                         s21_decimal *result, int *flag) {
  int sign_1, sign_2, sign_res, scale_1, scale_2, sub_scale, scale_res = 0,
                                                             break_button = 0;
  s21_big_decimal value_1_big = {0}, value_2_big = {0}, value_result_big = {0};
  s21_get_sign_get_scale_decimal(&value_1, &sign_1, &scale_1);
  s21_get_sign_get_scale_decimal(&value_2, &sign_2, &scale_2);
  s21_decimal_to_big_decimal(&value_1, &value_1_big);
  s21_decimal_to_big_decimal(&value_2, &value_2_big);
  sub_scale = scale_1 - scale_2;
  sign_res = sign_1 == sign_2 ? plus : minus;
  if (sub_scale < 0)
    s21_mult_by_10_n_big_decimal(&value_1_big, abs(sub_scale));
  else if (sub_scale > 0)
    s21_mult_by_10_n_big_decimal(&value_2_big, sub_scale);
  while (s21_is_less_big_decimal(&value_1_big, &value_2_big) &&
         s21_is_not_equal(value_1, (s21_decimal){0}))
    s21_mult_by_10_n_big_decimal(&value_1_big, 1), scale_res++;
  while (s21_check_big_decimal(&value_result_big) && !break_button) {
    s21_big_decimal temp = {0}, quotient = {0}, reminder = {0};
    s21_div_big_decimal(&value_1_big, &value_2_big, &quotient, &reminder);
    s21_add_big_decimal(value_result_big, quotient, &temp);
    value_result_big = temp;
    if (s21_is_equal_big_decimal(&reminder, &(s21_big_decimal){0}) ||
        scale_res > maximum_scale)
      break_button = 1;
    while (s21_is_less_big_decimal(&reminder, &value_2_big) && !break_button) {
      s21_mult_by_10_n_big_decimal(&reminder, 1);
      s21_mult_by_10_n_big_decimal(&value_result_big, 1);
      scale_res++;
    }
    value_1_big = reminder;
    if (scale_res > maximum_scale) {
      s21_big_decimal temp = {0}, quotient = {0}, reminder = {0};
      s21_div_big_decimal(&value_1_big, &value_2_big, &quotient, &reminder);
      s21_add_big_decimal(value_result_big, quotient, &temp);
      value_result_big = temp;
      break_button = 1;
    }
  }
  s21_bank_round(&value_result_big, &scale_res);
  if (!s21_check_big_decimal(&value_result_big))
    *flag = (sign_res == minus) ? 2 : 1;
  else
    s21_big_decimal_to_decimal(result, &value_result_big),
        s21_set_sign_scale(result, sign_res, scale_res);
}
