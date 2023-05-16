#include "s21_decimal.h"

/**
 * @brief
 * Основные функции больше и равно.
 * На их основе работают остальные функции.
 *
 * Функции больше и равно происходят в big_decimal
 * с полседующим переписыванием в decimal
 */

// Сравнение - больше
int s21_is_greater(s21_decimal num_1, s21_decimal num_2) {
  int output = 0;  // 0 - false, 1 - true
  int sign_1 = s21_get_bit(&num_1, 127);
  int sign_2 = s21_get_bit(&num_2, 127);

  if (num_1.bits[0] == 0 && num_1.bits[1] == 0 && num_1.bits[2] == 0 &&
      num_2.bits[0] == 0 && num_2.bits[1] == 0 && num_2.bits[2] == 0) {
    output = 0;
  } else {
    int result_of_big_decimal = 0;
    s21_big_decimal big_value_1 = {0};
    s21_big_decimal big_value_2 = {0};
    rewrite_decimal_to_big(&big_value_1, num_1);
    rewrite_decimal_to_big(&big_value_2, num_2);
    if (sign_1 < sign_2) {
      output = 1;
    } else if (sign_1 > sign_2) {
      output = 0;
    } else {
      result_of_big_decimal = s21_is_greater_big(big_value_1, big_value_2);
      if (result_of_big_decimal == 1) {
        if (!sign_1 && !sign_2) {
          output = 1;
        } else {
          output = 0;
        }
      } else if (result_of_big_decimal == 2) {
        if ((sign_1 && sign_2) || (!sign_1 && !sign_2)) output = 0;
        if ((sign_1 == 1 && sign_2 == 0)) output = 0;
        if ((sign_1 == 0 && sign_2 == 1)) output = 1;
      } else {
        if (sign_1 && sign_2) {
          output = 1;
        } else {
          output = 0;
        }
      }
    }
  }

  return output;
}

// Сравнение - равно
int s21_is_equal(s21_decimal num_1, s21_decimal num_2) {
  int output = 0;
  int sign_1 = s21_get_bit(&num_1, 127);
  int sign_2 = s21_get_bit(&num_2, 127);

  if (num_1.bits[0] == 0 && num_1.bits[1] == 0 && num_1.bits[2] == 0 &&
      num_2.bits[0] == 0 && num_2.bits[1] == 0 && num_2.bits[2] == 0) {
    output = 1;
  } else {
    int result_of_big_decimal = 0;
    s21_big_decimal big_value_1 = {0};
    s21_big_decimal big_value_2 = {0};
    rewrite_decimal_to_big(&big_value_1, num_1);
    rewrite_decimal_to_big(&big_value_2, num_2);

    result_of_big_decimal = s21_is_equal_big(big_value_1, big_value_2);
    if (result_of_big_decimal) {
      if (sign_1 != sign_2) output = 0;
      if (sign_1 == sign_2) output = 1;
    } else {
      output = 0;
    }
  }
  return output;
}

// Сравнение - меньше
int s21_is_less(s21_decimal num_1, s21_decimal num_2) {
  return (s21_is_greater(num_2, num_1));
}

// Сравнение - не равно
int s21_is_not_equal(s21_decimal num_1, s21_decimal num_2) {
  return (s21_is_equal(num_1, num_2) == 0 ? 1 : 0);
}

// Сравнение - больше или равно
int s21_is_greater_or_equal(s21_decimal num_1, s21_decimal num_2) {
  return s21_is_greater(num_1, num_2) || s21_is_equal(num_1, num_2);
}

// Сравнение - меньше или равно
int s21_is_less_or_equal(s21_decimal num_1, s21_decimal num_2) {
  return s21_is_less(num_1, num_2) || s21_is_equal(num_1, num_2);
}
