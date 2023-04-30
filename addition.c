#include "s21_decimal.h"

// add checks of overflow
// учет знака должен быть в этой функции
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int output = CONVERSATION_ERROR;
  int sign_1 = s21_get_bit(&value_1, 127);
  // int sign_2 = s21_get_bit(&value_2, 127);
  output = s21_sign_handle(&value_1, &value_2, result, 0);

  s21_big_decimal big_value_1 = {0};
  s21_big_decimal big_value_2 = {0};
  s21_big_decimal big_result = {0};
  rewrite_decimal_to_big(&big_value_1, value_1);
  rewrite_decimal_to_big(&big_value_2, value_2);
  s21_big_add(big_value_1, big_value_2, &big_result);

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
int s21_big_add(s21_big_decimal big_value_1, s21_big_decimal big_value_2,
                s21_big_decimal *big_result) {
  int output = CONVERSATION_OK;
  int sign_1 = s21_get_bit_big(&big_value_1, 223);  // будет основной знак

  int index = 0;
  char flag_bit_1 = 0;
  int num_1 = 0;
  int num_2 = 0;
  int power_of_1 = s21_get_power_of_big_decimal(big_value_1);
  int power_of_2 = s21_get_power_of_big_decimal(big_value_2);
  int power_of_result = 0;

  s21_big_decimal big_tmp = {0};
  s21_big_decimal zero_dec = {0};

  if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
    power_of_result =
        s21_normalize_big(&big_value_1, &big_value_2);  // нормализация
  }

  while (index != 192) {
    if (index >= 191 && flag_bit_1) {
      output = CONVERSATION_BIG;
    } else {
      num_1 = s21_get_bit_big(&big_value_1, index);
      num_2 = s21_get_bit_big(&big_value_2, index);
    }
    if ((num_1 && !num_2) || (!num_1 && num_2)) {
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
  int rewrite = 0;
  for (int i = 3; i <= 5; i++) {
    if (big_tmp.bits[i] == zero_dec.bits[i]) {
      rewrite++;
    }
  }
  if (rewrite == 3) *big_result = big_tmp;
  //   *result = tmp; - перед перезаписью надо сократить до нормального
  // постановка степени, нужна проверка на underflow
  s21_set_power_of_big_decimal(big_result, power_of_result);
  if (sign_1) s21_set_bit_1_big(&big_tmp, 223);  // постановка знака

  return output;
}