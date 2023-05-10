#include "s21_decimal.h"
// нужны тесты
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int output = CONVERSATION_OK;

  int sign_1 = s21_get_bit(&value_1, 127);
  int sign_2 = s21_get_bit(&value_2, 127);
  s21_set_dec_number_to_0(result);
  int result_of_big_decimal = 0;

  s21_set_bit_0(&value_2, 127);  // устанавливаем знаки в (+)
  s21_set_bit_0(&value_1, 127);

  s21_big_decimal big_value_1 = {{0}};
  s21_big_decimal big_value_2 = {{0}};
  s21_big_decimal big_result = {{0}};
  rewrite_decimal_to_big(&big_value_1, value_1);
  rewrite_decimal_to_big(&big_value_2, value_2);

  // умнжение на 0
  if (s21_is_equal_big(big_value_1, big_result) ||
      s21_is_equal_big(big_value_2, big_result)) {
    output = CONVERSATION_OK;
  } else {  // остальные случаи
    result_of_big_decimal =
        s21_big_mul(big_value_1, big_value_2, &big_result, 1);
    if (!result_of_big_decimal) {
      rewrite_from_big_decimal_to_decimal(big_result, result);
    } else if (result_of_big_decimal == 1) {
      if (sign_1 || sign_2) {
        output = CONVERSATION_SMALL;
      } else {
        output = CONVERSATION_BIG;
      }
    } else if (result_of_big_decimal == 2) {
      output = CONVERSATION_SMALL;
    }
  }

  if (sign_1 != sign_2) {  // постановка знака
    s21_set_bit_1(result, 127);
  } else {
    s21_set_bit_0(result, 127);
  }

  return output;
}

//------------------------Умножение big_decimal----------------------//

// func - переменная показывающая необходимо ли использовать функционал
// переполнения
int s21_big_mul(s21_big_decimal big_value_1, s21_big_decimal big_value_2,
                s21_big_decimal* result, int func) {
  int index = 0;
  int output = 1;  // поменял с end_mul
  s21_big_decimal step = {{0}};
  s21_set_big_dec_number_to_0(result);
  s21_big_decimal tmp = *result;
  s21_big_decimal big_10 = {{10, 0, 0, 0, 0, 0, 0}};
  int count_1 = 0, count_2 = 0;
  int power_of_1 = s21_get_power_of_big_decimal(big_value_1);
  int power_of_2 = s21_get_power_of_big_decimal(big_value_2);
  int power_of_result = power_of_1 + power_of_2;

  // для банковского округления
  s21_big_decimal fractional = {{0}};
  s21_big_decimal value_unsigned_truncated = {{0}};

  if ((power_of_1 || power_of_2)) {
    s21_normalize_big(&big_value_1, &big_value_2);  // нормализация
    power_of_result += abs(power_of_1 - power_of_2);
  }

  while (index != 63) {  // проверка что на что умножать будет
    count_1 += s21_get_bit_big(&big_value_1, index);
    count_2 += s21_get_bit_big(&big_value_2, index);
    index++;
  }
  index = 0;

  // само умножение путем сдвига
  if (count_1 < count_2) {
    while (index != 192 && output) {
      if (s21_get_bit_big(&big_value_1, index)) {
        step = big_value_2;
        output = s21_shift_bits_big(&step, index);
        s21_big_add(step, tmp, &tmp, 0);
        s21_set_big_dec_number_to_0(&step);
      }
      index++;
    }
  } else {
    while (index != 192 && output) {
      if (s21_get_bit_big(&big_value_2, index)) {
        step = big_value_1;
        output = s21_shift_bits_big(&step, index);
        s21_big_add(step, tmp, &tmp, 0);
        s21_set_big_dec_number_to_0(&step);
      }
      index++;
    }
  }

  if (func == 1) {
    if (power_of_1 || power_of_2) {
      power_of_result -= s21_truncate_zero_big(&tmp);
    }
    int rewrite = check_big_decimal(tmp);
    if (rewrite == 3) {
      *result = tmp;
    } else if (power_of_result) {
      while (power_of_result && rewrite != 3) {
        s21_div_big(tmp, big_10, &tmp);
        rewrite = check_big_decimal(tmp);

        s21_truncate_big(tmp, &value_unsigned_truncated);
        s21_sub_big(tmp, value_unsigned_truncated, &fractional, 0);
        tmp = s21_round_banking_big(value_unsigned_truncated, fractional);

        power_of_result--;
        rewrite = check_big_decimal(tmp);
      }
      if (!power_of_result && rewrite != 3) {
        return (output = CONVERSATION_BIG);
      } else {
        output = CONVERSATION_OK;
        *result = tmp;
      }
    } else {
      return (output = CONVERSATION_BIG);
    }
    if (power_of_result <= 28) {
      s21_set_power_of_big_decimal(result, power_of_result);
      output = CONVERSATION_OK;
    } else {
      // можно для деления что то подставить
      output = CONVERSATION_SMALL;
    }

  } else {
    *result = tmp;
  }
  return output;
}

/*
    работает
    1234.567 * 0.00000000000000000000000008
    0
    95, 90, 83, 70, 50 бит и 0.06^10 и любая приемлемая степень
    0.0000000000000008 * 12.34
    95 *  на обычные числа

*/
