#include "s21_decimal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int sign_value_1, sign_value_2, scale_value_1, scale_value_2, flag;
  s21_get_sign_get_scale_decimal(&value_1, &sign_value_1, &scale_value_1);
  s21_get_sign_get_scale_decimal(&value_2, &sign_value_2, &scale_value_2);
  int is_zero_1 = zero_check(value_1), is_zero_2 = zero_check(value_2);
  if (sign_value_1 > sign_value_2 && !(is_zero_1 && is_zero_2)) {
    flag = 1;
  } else if (sign_value_1 < sign_value_2) {
    flag = 0;
  } else {
    s21_big_decimal value_1_big = {0}, value_2_big = {0};
    s21_decimal_to_big_decimal(&value_1, &value_1_big);
    s21_decimal_to_big_decimal(&value_2, &value_2_big);
    int sub_scale = scale_value_1 - scale_value_2;
    if (sub_scale < 0) {
      s21_mult_by_10_n_big_decimal(&value_1_big, abs(sub_scale));
    } else if (sub_scale > 0) {
      s21_mult_by_10_n_big_decimal(&value_2_big, abs(sub_scale));
    }
    if (sign_value_1 == 0) {
      flag = s21_is_less_big_decimal(&value_1_big, &value_2_big);
    } else {
      flag = s21_is_greater_big_decimal(&value_1_big, &value_2_big);
    }
  }
  return flag;
}

int s21_is_less_big_decimal(s21_big_decimal *value_1,
                            s21_big_decimal *value_2) {
  int flag = 2;
  for (int i = count_element_in_big_decimal - 1; i >= 0 && flag == 2; i--) {
    for (int j = sign_bits; j >= 0 && flag == 2; j--) {
      int bit_value_1 = (value_1->bits[i] >> j) & 1;
      int bit_value_2 = (value_2->bits[i] >> j) & 1;
      if (bit_value_1 != bit_value_2) {
        if (bit_value_1 < bit_value_2) {
          flag = 1;
        } else {
          flag = 0;
        }
      }
    }
  }
  if (flag == 2) flag = 0;
  return flag;
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  int flag = 0;
  if (s21_is_equal(value_1, value_2) == 0) {
    flag = s21_is_less(value_1, value_2) == 0 ? 1 : 0;
  }
  return flag;
}

int s21_is_greater_big_decimal(s21_big_decimal *value_1,
                               s21_big_decimal *value_2) {
  int flag = 2;
  for (int i = count_element_in_big_decimal - 1; i >= 0 && flag == 2; i--) {
    for (int j = sign_bits; j >= 0 && flag == 2; j--) {
      int bit_value_1 = (value_1->bits[i] >> j) & 1;
      int bit_value_2 = (value_2->bits[i] >> j) & 1;
      if (bit_value_1 != bit_value_2) {
        if (bit_value_1 > bit_value_2) {
          flag = 1;
        } else {
          flag = 0;
        }
      }
    }
  }
  if (flag == 2) flag = 0;
  return flag;
}

int s21_is_equal_big_decimal(s21_big_decimal *value_1,
                             s21_big_decimal *value_2) {
  int flag = 1;
  for (int i = count_element_in_big_decimal - 1; i >= 0 && flag == 1; i--) {
    for (int j = sign_bits; j >= 0 && flag == 1; j--) {
      int bit_value_1 = (value_1->bits[i] >> j) & 1;
      int bit_value_2 = (value_2->bits[i] >> j) & 1;
      if (bit_value_1 != bit_value_2) {
        flag = 0;
      }
    }
  }
  return flag;
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int sign_value_1, sign_value_2, scale_value_1, scale_value_2, flag;
  s21_get_sign_get_scale_decimal(&value_1, &sign_value_1, &scale_value_1);
  s21_get_sign_get_scale_decimal(&value_2, &sign_value_2, &scale_value_2);
  int is_zero_1 = zero_check(value_1), is_zero_2 = zero_check(value_2);
  if (((sign_value_1 != sign_value_2) || (scale_value_1 != scale_value_2)) &&
      !(is_zero_1 && is_zero_2)) {
    flag = 0;
  } else {
    s21_big_decimal value_1_big = {0}, value_2_big = {0};
    s21_decimal_to_big_decimal(&value_1, &value_1_big);
    s21_decimal_to_big_decimal(&value_2, &value_2_big);
    flag = s21_is_equal_big_decimal(&value_1_big, &value_2_big);
  }
  return flag;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_greater(value_1, value_2);
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_less(value_1, value_2);
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_equal(value_1, value_2);
}

int zero_check(s21_decimal is_zero) {
  return !(is_zero.bits[0] | is_zero.bits[1] | is_zero.bits[2]);
}