#include "s21_decimal.h"

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int output = CONVERSATION_ERROR;

  if (result) {
    output = CONVERSATION_OK;
    int power = s21_get_power_of_decimal(value);
    s21_set_power_of_decimal(&value, 0);
    while (power) {
      s21_div_decimal_by_10(&value);  // заменить после на нормальное деление
      power--;
    }
    *result = value;
  }
  return output;
}
// убрать в другие функции
void s21_truncate_zero(s21_decimal *value, int count_zero) {
  if (value) {
    while (count_zero) {
      s21_div_decimal_by_10(value);  // заменить после на нормальное деление
      count_zero--;
    }
  }
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  int output = CONVERSATION_ERROR;
  if (result) {
    if (s21_get_bit(&value, 127)) {
      s21_set_bit_0(&value, 127);
    } else {
      s21_set_bit_1(&value, 127);
    }
    *result = value;
    output = CONVERSATION_OK;
  }
  return output;
}  // Надо ли проверять на корректность decimal или нет?