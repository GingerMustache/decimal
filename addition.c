#include "s21_decimal.h"

// add checks of overflow
// учет знака должен быть в этой функции
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int output = CONVERSATION_ERROR;
  int sign_1 = s21_get_bit(&value_1, 127);
  // int sign_2 = s21_get_bit(&value_2, 127);
  output = s21_sign_handle(&value_1, &value_2, result, 0);

  // s21_big_decimal big_value_1 = {0};
  // s21_big_decimal big_value_2 = {0};
  // s21_big_decimal big_result = {0};
  // rewrite_decimal_to_big(&big_value_1, value_1);
  // rewrite_decimal_to_big(&big_value_2, value_2);
  // s21_big_add(big_value_1, big_value_2, &big_result, 1);

  if (output == 2) {
    output = CONVERSATION_OK;

    int index = 0;
    char flag_bit_1 = 0;
    int num_1 = 0;
    int num_2 = 0;
    int power_of_1 = s21_get_power_of_decimal(value_1);
    int power_of_2 = s21_get_power_of_decimal(value_2);
    int power_of_result = 0;
    s21_decimal tmp = {0};

    if (sign_1) s21_set_bit_1(&tmp, 127);  // постановка знака чисел

    // необходимо учесть банковское округление либо в нормализации, либо перед
    if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
      power_of_result = s21_normalize(&value_1, &value_2);  // нормализация
    }

    while (index != 97) {
      if (index >= 96 && flag_bit_1) {
        output = CONVERSATION_BIG;
      } else {
        num_1 = s21_get_bit(&value_1, index);
        num_2 = s21_get_bit(&value_2, index);
      }
      if (output == CONVERSATION_OK) {
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
      // постановка степени, нужна проверка на underflow
      s21_set_power_of_decimal(result, power_of_result);
    }
  }

  return output;
}

//------------------------Сложение big_decimal----------------------//
// тут нет учета знака
// func - переменная показывающая откуда пришел вызов функции, если 1 из
// сложения, если нет то откуда угодно
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

  s21_big_decimal big_tmp = {0};

  if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
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
    int rewrite = check_big_decimal(big_tmp);
    if (rewrite == 3) {
      *big_result = big_tmp;
    } else if (power_of_result) {
      while (power_of_result || rewrite != 3) {
        s21_div_decimal_by_10_big(&big_tmp);
        s21_round_big(big_tmp, &big_tmp);
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