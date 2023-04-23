#include "s21_decimal.h"

// need to add comparence of exp part
// add sign comparece
int s21_is_greater(s21_decimal num_1, s21_decimal num_2) {
  int res_1 = 0, res_2 = 0;
  int i = 95;
  int output = 0;
  int sign_1 = s21_get_bit(&num_1, 127);
  int sign_2 = s21_get_bit(&num_2, 127);
  // до if теобходимо разобраться со степенью

  if (sign_1 < sign_2) {
    output = 1;
  } else if (sign_1 > sign_2) {
    output = 0;
  } else {
    while (res_1 == res_2 && i != -1) {
      res_1 = s21_get_bit(&num_1, i);
      res_2 = s21_get_bit(&num_2, i);
      i--;
    }
    if (sign_1 == 0 && sign_2 == 0) {  // если числа положительные
      if (res_1 > res_2)
        output = 1;
      else
        output = 0;
    } else {  // если числа отрицательные
      if (res_1 < res_2)
        output = 1;
      else
        output = 0;
    }
  }

  return output;
}
