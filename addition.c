#include "s21_decimal.h"
// нужны тесты
// все вычисления происходят в big_decimal и потом пепереписываются в decimal
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int output = CONVERSATION_OK;
  int sign_1 = s21_get_bit(&value_1, 127);
  s21_decimal zero_decimal = {0};
  output = s21_sign_handle(&value_1, &value_2, result, 0);

  if (output == 2) {
    // если хоть один максимум и второй не 0 -> на выход
    if (s21_is_max_decimal(value_1) || s21_is_max_decimal(value_2)) {
      if (!s21_is_max_decimal(value_1) && s21_is_equal(value_2, zero_decimal)) {
        return (output = CONVERSATION_BIG);
      } else if (!s21_is_max_decimal(value_2) &&
                 s21_is_equal(value_1, zero_decimal)) {
        return (output = CONVERSATION_BIG);
      } else {
        output = CONVERSATION_OK;
      }
    } else {
      output = CONVERSATION_OK;
    }
    if (output == CONVERSATION_OK) {
      s21_set_dec_number_to_0(result);
      int big_decimal_output = 0;
      s21_decimal tmp = {0};

      if (sign_1) s21_set_bit_1(&tmp, 127);  // постановка знака чисел

      s21_big_decimal big_value_1 = {0};
      s21_big_decimal big_value_2 = {0};
      s21_big_decimal big_result = {0};
      rewrite_decimal_to_big(&big_value_1, value_1);
      rewrite_decimal_to_big(&big_value_2, value_2);

      big_decimal_output =
          s21_big_add(big_value_1, big_value_2, &big_result, 1);
      if (!big_decimal_output) {
        rewrite_from_big_decimal_to_decimal(big_result, &tmp);
        *result = tmp;
        output = CONVERSATION_OK;
      } else if (big_decimal_output == 1) {
        output = CONVERSATION_BIG;
      } else if (big_decimal_output == 2) {
        output = CONVERSATION_SMALL;
      }
    }
  }

  return output;
}

//------------------------Сложение big_decimal----------------------//
// тут нет учета знака
// func - переменная показывающая необходимо ли использовать функционал
// переполнения
int s21_big_add(s21_big_decimal big_value_1, s21_big_decimal big_value_2,
                s21_big_decimal *big_result, int func) {
  int output = CONVERSATION_OK;

  int index = 0;
  int flag_bit_1 = 0;
  int num_1 = 0;
  int num_2 = 0;
  int power_of_1 = s21_get_power_of_big_decimal(big_value_1);
  int power_of_2 = s21_get_power_of_big_decimal(big_value_2);
  int power_of_result = 0;

  s21_big_decimal fractional = {0};
  s21_big_decimal value_unsigned_truncated = {0};

  s21_big_decimal big_tmp = {0};
  s21_big_decimal big_10 = {10, 0, 0, 0, 0, 0, 0};
  ;

  if (power_of_1 || power_of_2) {
    power_of_result =
        s21_normalize_big(&big_value_1, &big_value_2);  // нормализация
  }

  while (index != 192 && output == CONVERSATION_OK) {
    if (index >= 191 && flag_bit_1) {
      output = CONVERSATION_BIG;
    } else {
      num_1 = s21_get_bit_big(&big_value_1, index);
      num_2 = s21_get_bit_big(&big_value_2, index);
    }
    if (((num_1 && !num_2) || (!num_1 && num_2)) && output == CONVERSATION_OK) {
      if (flag_bit_1) {
        s21_set_bit_0_big(&big_tmp, index);
        flag_bit_1 = 1;
      } else {
        s21_set_bit_1_big(&big_tmp, index);
      }
    } else if (!num_1 && !num_2) {
      if (flag_bit_1) {
        s21_set_bit_1_big(&big_tmp, index);
        flag_bit_1 = 0;
      } else {
        s21_set_bit_0_big(&big_tmp, index);
      }
    } else {  // (num_1 && num_2)
      if (flag_bit_1) {
        s21_set_bit_1_big(&big_tmp, index);
        flag_bit_1 = 1;
      } else {
        s21_set_bit_0_big(&big_tmp, index);
        flag_bit_1 = 1;
      }
    }
    index++;
  }
  if (func == 1) {
    s21_print_big_decimal_number(&big_tmp);
    int rewrite = check_big_decimal(big_tmp);
    if (rewrite == 3) {
      *big_result = big_tmp;
    } else if (power_of_result) {
      while (power_of_result && rewrite != 3) {
        s21_div_big(big_tmp, big_10, &big_tmp);
        s21_print_big_decimal_number(&big_tmp);
        // s21_round_big(big_tmp, &big_tmp);
        s21_truncate_big(big_tmp, &value_unsigned_truncated);
        s21_sub_big(big_tmp, value_unsigned_truncated, &fractional, 1);
        s21_print_big_decimal_number(&big_tmp);
        big_tmp = s21_round_banking_big(value_unsigned_truncated, fractional);
        s21_print_big_decimal_number(&big_tmp);
        rewrite = check_big_decimal(big_tmp);
        power_of_result--;
      }
      if (!power_of_result && rewrite != 3) {
        output = CONVERSATION_BIG;
      } else {
        output = CONVERSATION_OK;
        *big_result = big_tmp;
      }
    } else {
      output = CONVERSATION_BIG;
    }

    if (output == CONVERSATION_OK)
      s21_set_power_of_big_decimal(big_result, power_of_result);
  } else {
    *big_result = big_tmp;
  }

  return output;
}

int check_big_decimal(s21_big_decimal big_tmp) {
  int rewrite = 0;
  s21_big_decimal zero_dec = {0};
  for (int i = 3; i < 6; i++) {
    if (big_tmp.bits[i] == zero_dec.bits[i]) {
      rewrite++;
    }
  }
  return (rewrite);
}