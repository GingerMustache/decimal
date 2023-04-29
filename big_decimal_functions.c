#include "s21_decimal.h"

int s21_big_mul(s21_big_decimal big_value_1, s21_big_decimal big_value_2,
                s21_big_decimal *result) {
  int output = CONVERSATION_OK;

  if (result) {
    int count_1 = 0, count_2 = 0;
    int index = 0;
    // int sign_1 = s21_get_bit(&value_1, 223);
    // int sign_2 = s21_get_bit(&value_2, 223);
    s21_big_decimal step = {0};
    s21_set_big_dec_number_to_0(result);
    s21_big_decimal tmp = *result;
    // s21_big_decimal buffer = {0};

    // int power_of_result = 0;
    // степень пропадает при вычислениях и появляется *result = tmp
    // степепнь сохраняется  в нормализации
    // int power_of_1 = s21_get_power_of_decimal(value_1);
    // int power_of_2 = s21_get_power_of_decimal(value_2);
    // s21_big_decimal big_value_1 = {0};
    // s21_big_decimal big_value_2 = {0};
    // rewrite_decimal_to_big(&big_value_1, value_1);
    // rewrite_decimal_to_big(&big_value_2, value_2);
    // int res_of_summ = 0; принимал s21_add

    s21_set_bit_0_big(&big_value_2, 223);  // устанавливаем знаки в (+)
    s21_set_bit_0_big(&big_value_1, 223);

    // if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
    //   s21_normalize(&value_1, &value_2);  // нормализация
    // }

    while (index != 90) {  // надо посмотреть потом
      count_1 += s21_get_bit_big(&big_value_1, index);
      count_2 += s21_get_bit_big(&big_value_2, index);
      index++;
    }
    index = 0;
    // case 1, где меньше едииц на то и умножаем
    if (count_1 < count_2) {
      while (index != 191) {
        if (s21_get_bit_big(&big_value_1, index)) {
          step = big_value_2;
          s21_shift_bits_big(&step, index);
          s21_big_add(step, tmp, &tmp);
          s21_set_big_dec_number_to_0(&step);
        }
        index++;
      }
    } else {
      while (index != 191) {
        if (s21_get_bit_big(&big_value_2, index)) {
          step = big_value_1;
          s21_shift_bits_big(&step, index);
          s21_big_add(step, tmp, &tmp);
          s21_set_big_dec_number_to_0(&step);
        }
        index++;
      }
    }
    *result = tmp;
    if (output == CONVERSATION_OK) {
      // output = CONVERSATION_OK;  // тут, до проверки на overflow
      // обрезает нули при необходимости ??
      // // добавь в функциюнормальное деление
      // if (power_of_1 && power_of_2) {
      //   s21_truncate_zero_big(result, abs(power_of_1 - power_of_2));
      // }
      // s21_set_power_of_big_decimal(
      //     result,
      //     power_of_1 + power_of_2);  // постановка степени
      // if (sign_1 != sign_2) {        // постановка знака
      //   s21_set_bit_1_big(result, 223);
      // } else {
      //   s21_set_bit_0_big(result, 223);
      // }
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

void s21_truncate_zero_big(s21_big_decimal *value, int count_zero) {
  if (value) {
    while (count_zero) {
      //   s21_div_decimal_by_10(value);  // заменить после на нормальное
      //   деление
      count_zero--;
    }
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