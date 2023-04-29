#include "s21_decimal.h"

int s21_normalize_big(s21_big_decimal *num_1, s21_big_decimal *num_2) {
  int power_num_1 = s21_get_power_of_big_decimal(*num_1);
  int power_num_2 = s21_get_power_of_big_decimal(*num_2);
  s21_set_power_of_big_decimal(num_1, 0);  // ставим степени чисел в 0
  s21_set_power_of_big_decimal(num_2, 0);
  s21_big_decimal bit_number_10 = {10, 0, 0, 0, 0, 0, 0};
  // printf("\n-decimal_numbers from normalize-\n");
  // printf("bit_num_10\n");
  // s21_print_decimal_number(&bit_number_10);
  // printf("1st\n");
  // s21_print_decimal_number(num_1);
  // printf("2nd\n");
  // s21_print_decimal_number(num_2);

  if (power_num_1 < power_num_2) {
    while (power_num_2 - power_num_1) {
      s21_big_mul(*num_1, bit_number_10, num_1);
      power_num_1++;
    }
    return (power_num_1);
  } else if (power_num_2 < power_num_1) {
    while (power_num_1 - power_num_2) {
      s21_big_mul(*num_2, bit_number_10, num_2);
      power_num_2++;
    }
    return (power_num_2);
  }
  return (power_num_1);
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