#include "s21_decimal.h"

// need to add comparence of exp part
int s21_is_greater(s21_decimal num_1, s21_decimal num_2) {
  int res_1 = 0, res_2 = 0;
  int i = 95;
  int output = 0;
  while (res_1 == res_2) {
    res_1 = s21_get_bit(&num_1, i);
    res_2 = s21_get_bit(&num_2, i);
    i--;
  }

  if (res_1 > res_2)
    output = 1;
  else
    output = 0;

  return output;
}
