#include "s21_decimal.h"

// need to add comparence of exp part
int s21_is_greater(s21_decimal num_1, s21_decimal num_2) {
  int res_1 = 0, res_2 = 0;
  int i = 95;
  int output = 0;  // 0 - false, 1 - true
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

int s21_is_less(s21_decimal num_1, s21_decimal num_2) {
  return (s21_is_greater(num_2, num_1));
}

int s21_is_equal(s21_decimal num_1, s21_decimal num_2) {
  int output = 0;
  // int power_1 = s21_get_power_of_decimal(num_1);
  // int power_2 = s21_get_power_of_decimal(num_2);

  // необходимо добавить нормализацию
  // 0.1 и степень 1,  должен быть == 1

  output = num_1.bits[0] == num_2.bits[0] && num_1.bits[1] == num_2.bits[1] &&
           num_1.bits[2] == num_2.bits[2] && num_1.bits[3] == num_2.bits[3];
  return output;
}

int s21_is_not_equal(s21_decimal num_1, s21_decimal num_2) {
  return (s21_is_equal(num_1, num_2) == 0 ? 1 : 0);
}

int s21_is_greater_or_equal(s21_decimal num_1, s21_decimal num_2) {
  return s21_is_greater(num_1, num_2) || s21_is_equal(num_1, num_2);
}

int s21_is_less_or_equal(s21_decimal num_1, s21_decimal num_2) {
  return s21_is_less(num_1, num_2) || s21_is_equal(num_1, num_2);
}