#include "s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  int scale = 0, sign = 0, flag = 0, ostatok = 0;
  if (s21_check_decimal(&value) || result == NULL)
    flag = 1;
  else {
    clearData(result), s21_get_sign_get_scale_decimal(&value, &sign, &scale);
    value.bits[3] = 0;
    s21_decimal ten = {10}, temp = {0};
    while (scale) {
      s21_decimal test = {{s21_dec_mod_10(value)}};
      if (test.bits[0] != 0) ostatok = 1;
      s21_sub(value, test, &temp);
      value = temp, s21_div(value, ten, &temp), scale--;
      value = temp;
    }
    s21_decimal one = {{1}};
    if (sign && ostatok) s21_add(value, one, &value);
    setDecimalSign(&value, sign), *result = value;
  }
  return flag;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int scale = 0, sign = 0, flag = 0, ostatok = 0, last_simb = 0;
  if (s21_check_decimal(&value) || result == NULL)
    flag = 1;
  else {
    clearData(result), s21_get_sign_get_scale_decimal(&value, &sign, &scale);
    value.bits[3] = 0;
    s21_decimal ten = {10};
    while (scale) {
      scale--;
      s21_decimal test = {{s21_dec_mod_10(value)}};
      if (scale && test.bits[0] != 0) ostatok = 1;
      s21_sub(value, test, &value);
      s21_div(value, ten, &value);
      last_simb = test.bits[0];
    }
    s21_decimal one = {{1}};
    if (last_simb > 5 || last_simb == 5 && ((ostatok) || (value.bits[0] & 1)))
      s21_add(value, one, &value);
    setDecimalSign(&value, sign), *result = value;
  }
  return flag;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int sign, scale, flag = 0;
  if (s21_check_decimal(&value) || result == NULL)
    flag = 1;
  else {
    s21_big_decimal value_big = {0};
    s21_get_sign_get_scale_decimal(&value, &sign, &scale);
    s21_decimal_to_big_decimal(&value, &value_big);
    while (scale) {
      s21_big_decimal quotient = {0}, reminder = {0};
      s21_div_big_decimal(&value_big, &(s21_big_decimal){.bits = {[0] = 10}},
                          &quotient, &reminder);
      value_big = quotient;
      scale--;
    }
    s21_big_decimal_to_decimal(result, &value_big);
    s21_set_sign_scale(result, sign, scale);
    return flag;
  }
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  int negate = 0, flag = 0;
  if (s21_check_decimal(&value) || result == NULL)
    flag = 1;
  else {
    *result = value;
    bit3 *parting = (bit3 *)&result->bits[3];
    parting->parts.sign = !parting->parts.sign;
    return negate;
  }
}