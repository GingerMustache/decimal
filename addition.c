#include "s21_decimal.h"

// add checks of overflow
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int output = 0;
  int index = 0;
  char flag_bit_1 = 0;
  int num_1 = 0;
  int num_2 = 0;
  s21_decimal tmp = {0};

  while (index != 96) {
    num_1 = s21_get_bit(&value_1, index);
    num_2 = s21_get_bit(&value_2, index);

    if ((num_1 && !num_2) || (!num_1 && num_2)) {
      if (flag_bit_1) {
        s21_set_bit_0(&tmp, index);
        flag_bit_1 = 1;
      } else {
        s21_set_bit_1(&tmp, index);
      }
    } else if (!num_1 && !num_2) {
      if (flag_bit_1) {
        s21_set_bit_1(&tmp, index);
        flag_bit_1 = 0;
      } else {
        s21_set_bit_0(&tmp, index);
      }
    } else {  // (num_1 && num_2)
      if (flag_bit_1) {
        s21_set_bit_1(&tmp, index);
        flag_bit_1 = 1;
      } else {
        s21_set_bit_0(&tmp, index);
        flag_bit_1 = 1;
      }
    }
    index++;
  }
  *result = tmp;

  return output;
}