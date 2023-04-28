#include "s21_decimal.h"
//------------------------Сложение----------------------//
int s21_big_add(s21_decimal value_1, s21_decimal value_2,
                s21_big_decimal *big_result) {
  int output = CONVERSATION_ERROR;
  int sign_1 = s21_get_bit(&value_1, 127);
  s21_decimal result = {0};  // результат для обычного decimal
  // int sign_2 = s21_get_bit(&value_2, 127);
  output = s21_sign_handle(&value_1, &value_2, &result, 0);
  s21_big_decimal big_value_1 = {0};
  s21_big_decimal big_value_2 = {0};
  rewrite_decimal_to_big(&big_value_1, value_1);
  rewrite_decimal_to_big(&big_value_2, value_2);

  if (output == 2) {
    output = CONVERSATION_OK;

    int index = 0;
    char flag_bit_1 = 0;
    int num_1 = 0;
    int num_2 = 0;
    // int power_of_1 = s21_get_power_of_decimal(value_1);
    // int power_of_2 = s21_get_power_of_decimal(value_2);
    int power_of_result = 0;
    // s21_decimal tmp = {0};
    s21_big_decimal big_tmp = {0};

    if (sign_1) s21_set_bit_1_big(&big_tmp, 223);  // постановка знака чисел

    // необходимо учесть банковское округление либо в нормализации, либо перед
    // if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
    //   power_of_result = s21_normalize(&value_1, &value_2);  // нормализация
    // }

    // нормализацию надо переписать на big_decimal

    while (index != 192) {
      if (index >= 191 && flag_bit_1) {
        output = CONVERSATION_BIG;
      } else {
        num_1 = s21_get_bit_big(&big_value_1, index);
        num_2 = s21_get_bit_big(&big_value_2, index);
      }
      if (output == CONVERSATION_OK) {
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
      //   *result = tmp; - перед перезаписью надо сократить до нормального
      *big_result = big_tmp;
      // постановка степени, нужна проверка на underflow
      s21_set_power_of_big_decimal(big_result, power_of_result);
    }
  }

  return output;
}

//------------------------Доп. функции----------------------//
void rewrite_decimal_to_big(s21_big_decimal *big_decimal, s21_decimal decimal) {
  int power_of_decimal = s21_get_power_of_decimal(decimal);

  for (int i = 0; i != 96; i++) {
    if (s21_get_bit(&decimal, i)) {
      s21_set_bit_1_big(big_decimal, i);
    } else {
      s21_set_bit_0_big(big_decimal, i);
    }
  }
  s21_set_power_of_big_decimal(big_decimal, power_of_decimal);
}

int s21_get_power_of_big_decimal(s21_big_decimal src) {
  int index = 208;
  int result = 0;
  int scale = 0;
  while (index != 215) {
    if (s21_get_bit_big(&src, index)) {
      result += pow(2, scale);
    }
    index++;
    scale++;
  }
  return result;
}

void s21_set_bits_from_int_to_big_decimal(int src, s21_big_decimal *dst,
                                          int which_int_part_to_fill) {
  int exp = 0;
  src = fabs((double)src);
  while (src >= pow(2, exp)) exp++;
  while (src) {
    if (src - pow(2, exp) < 0) {
      s21_set_bit_0_big(dst, exp + which_int_part_to_fill);
    } else {
      src -= pow(2, exp);
      s21_set_bit_1_big(dst, exp + which_int_part_to_fill);
    }
    exp--;
  }
}

void s21_set_power_of_big_decimal(s21_big_decimal *src, int power) {
  if (power < 29) {  // не понял пока сколько надо, но точно больше
    for (int i = 208; i < 215; i++) {  // обнуление
      s21_set_bit_0_big(src, i);
    }
    s21_set_bits_from_int_to_big_decimal(power, src, 208);  // заполнение
  }
}

//------------------------Управление битами----------------------//
void s21_set_bit_1_big(s21_big_decimal *dec_num, int index) {
  int getRow = s21_get_row(index);
  int getCol = s21_get_col(index);
  dec_num->bits[getRow] |= (1 << getCol);
}

void s21_set_bit_0_big(s21_big_decimal *dec_num, int index) {
  int getRow = s21_get_row(index);
  int getCol = s21_get_col(index);
  dec_num->bits[getRow] &= ~(1 << getCol);
}

int s21_get_bit_big(s21_big_decimal *dec_num, int index) {
  int getRow = s21_get_row(index);
  int getCol = s21_get_col(index);
  return (dec_num->bits[getRow] & (1 << getCol)) != 0;
}