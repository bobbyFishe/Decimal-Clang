#include <math.h>

#include "s21_decimal.h"
int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int flag = 0;
  if (dst == NULL) flag = 1;
  if (!flag) {
    *dst = (s21_decimal){0};
    if (src < 0) {
      src = -src, setDecimalSign(dst, 1);
    }
    dst->bits[0] = src;
  }
  return flag;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int flag = 0, scale = 0, sign = 0;
  if (dst != NULL) {
    *dst = 0;
    s21_get_sign_get_scale_decimal(&src, &sign, &scale);
    src.bits[3] = 0;
    s21_decimal result = {0};
    while (scale) {
      scale--;
      s21_decimal test = {{s21_dec_mod_10(src)}};
      s21_sub(src, test, &result);
      s21_div(result, (s21_decimal){10}, &result);
      src = result;
    }
    if ((unsigned int)src.bits[1] > 0 || (unsigned int)src.bits[2] > 0 ||
        (unsigned int)src.bits[0] > 2147483647)
      flag = 1;
    else {
      *dst = src.bits[0];
      *dst = (sign) ? -*dst : *dst;
    }
  }
  return flag;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int scale = 0, sign = 0, flag = 0;
  if (dst == NULL) flag = 1;
  if (!flag)
    s21_get_sign_get_scale_decimal(&src, &sign, &scale),
        *dst = ((unsigned int)src.bits[0] +
                (unsigned int)src.bits[1] * pow(2, 32) +
                (unsigned int)src.bits[2] * pow(2, 64)) /
               pow(10, scale) * pow(-1, sign);
  return flag;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  clearData(dst);
  int sign = 0, exponent = 0, flag = 0;
  if (src < 0) sign = !sign, src *= (-1);
  while (src != floor(src)) src *= 10, exponent++;
  while (floor((src)) != 0 && exponent > 28) exponent--, src /= 10;
  if (exponent > 28) flag = 1;
  if (!flag) {
    while (src > 9999999) src /= 10, exponent--;
    src = round(src);
    dst->bits[0] = (int)src;
    while (exponent < 0)
      exponent++, flag = s21_mul(*dst, (s21_decimal){10}, dst);
  }
  if (!flag) {
    bit3 *signscale = (bit3 *)&dst->bits[3];
    signscale->parts.sign = sign;
    signscale->parts.scale = exponent;
  }
  return flag;
}

int s21_dec_mod_10(s21_decimal src) {
  int count = 0, ost = 0;
  for (int i = 0; i < count_element_in_decimal; i++)
    for (int j = 0; j < count_bits_in_int; j++) {
      int bit = src.bits[i] >> j & 1;
      if (bit) {
        int a = 2;
        switch (count % 4) {
          case 1:
            a = 2;
            break;
          case 2:
            a = 4;
            break;
          case 3:
            a = 8;
            break;
          default:
            a = 6;
            break;
        }
        if (i == 0 && j == 0) a = 1;
        ost += a;
      }
      count++;
    }
  ost = ost % 10;
  return ost;
}

void setDecimalSign(s21_decimal *decimal, int sign) {
  unsigned int signMask = 1u << 31;
  if (sign != 0) {
    decimal->bits[3] |= signMask;
  } else {
    decimal->bits[3] &= ~signMask;
  }
}
