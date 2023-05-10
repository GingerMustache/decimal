#include "s21_decimal.h"

int s21_get_row(int bit) { return bit / 32; }
int s21_get_col(int bit) { return bit % 32; }

void s21_set_sign_of_int_and_float_number(s21_decimal *dec_num, float num_1,
                                          int case_of_type) {
  if (case_of_type == 1) {
    // if ((int)num_1 < 0) {
    //   if (num_1 != INT_MIN) {
    //     dec_num->bits[3] |= (1 << 31);
    //   } else {
    //     dec_num->bits[3] |= (0 << 31);
    //   }
    // }
  } else {
    long int i = two_32 / 2;
    int float_in_32bit = *(int *)&num_1;
    if (case_of_type == 2) {
      if (float_in_32bit & i) {
        s21_set_bit_1(dec_num, 31);
      }
    } else {
      if (float_in_32bit & i) {
        s21_set_bit_1(dec_num, 127);
      }
    }
  }
}

// void s21_truncate_zero(s21_decimal *value, int count_zero) {
//   if (value) {
//     while (count_zero) {
//       s21_div_decimal_by_10(value);  // заменить после на нормальное деление
//       count_zero--;
//     }
//   }
// }

void s21_cut_exp(s21_decimal *a, int exp) {
  for (int i = exp; i > 0; i--) {
    s21_div_decimal_by_10(a);  // заменить потом на нормальное деление
  }
  s21_set_power_of_decimal(a, 0);
}

int s21_get_bits(unsigned int bits, unsigned int num) {
  unsigned int mask = 1 << num;
  return (bits & mask) ? 1 : 0;
}

int s21_get_bit(s21_decimal *dec_num, int index) {
  int getRow = s21_get_row(index);
  int getCol = s21_get_col(index);
  return (dec_num->bits[getRow] & (1 << getCol)) != 0;
}

// int s21_get_bit_from_not_decimal(float src, int index) {
//   int getCol = s21_get_col(index);
//   return ((int)src & (1 << getCol)) != 0;
// }

void s21_set_bit_1(s21_decimal *dec_num, int index) {
  int getRow = s21_get_row(index);
  int getCol = s21_get_col(index);
  dec_num->bits[getRow] |= (1 << getCol);
}

void s21_set_bit_0(s21_decimal *dec_num, int index) {
  int getRow = s21_get_row(index);
  int getCol = s21_get_col(index);
  dec_num->bits[getRow] &= ~(1 << getCol);
}

void s21_set_dec_number_to_0(s21_decimal *src_num) {
  memset(&(src_num->bits), 0, sizeof(src_num->bits));
}

void s21_set_big_dec_number_to_0(s21_big_decimal *src_num) {
  memset(&(src_num->bits), 0, sizeof(src_num->bits));
}

int s21_is_decimal_0(s21_decimal dec_num) {
  s21_decimal zero_decimal = {0};
  return s21_is_equal(zero_decimal, dec_num);
}

// int s21_inverse_bit(s21_decimal *dec_num, int index) {
//   int getRow = s21_get_row(index);
//   int getCol = s21_get_col(index);
//   return dec_num->bits[getRow] ^= (1 << getCol);
// }

// int s21_get_float_bit(int num, int index) { return (num & (1 << index)) != 0;
// };

// надо переписать, как для big_decimal сделано
void s21_div_decimal_by_10(s21_decimal *num) {
  unsigned long long rem = 0, tmp;
  for (int i = 2; i >= 0; i--) {
    tmp = 0;
    tmp = (unsigned long long)num->bits[i] + rem;
    num->bits[i] = (unsigned int)(tmp / 10);
    rem = (tmp % 10) << 32;
  }
}

// int s21_mul_decimal_by_10(s21_decimal *num) {
//   s21_decimal decimal_10 = {10, 0, 0, 0};
//   int res = s21_mul(*num, decimal_10, num);
//   // printf("res of  = %d\n", res);
//   if (!res)  // 0
//     return (0);
//   else
//     return (1);
// }

int s21_get_power_of_decimal(s21_decimal src) {
  int index = 112;
  int result = 0;
  int scale = 0;
  while (index != 120) {
    if (s21_get_bit(&src, index)) {
      result += pow(2, scale);
    }
    index++;
    scale++;
  }
  if (result > 28) result = 0;
  return result;
}

// s21_decimal s21_decimal_get_inf(void) {
//   s21_decimal result = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0x7FFFFFFF}};

//   return result;
// }

// int s21_is_max_decimal(s21_decimal num) {
//   s21_decimal over = {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0};
//   return s21_is_equal(num, over);
// }

void s21_set_dec_number_to_1(s21_decimal *src_num) {
  s21_decimal decimal_1 = {1};
  *src_num = decimal_1;
}

void s21_set_power_of_decimal(s21_decimal *src, int power) {
  if (power < 29) {
    for (int i = 112; i < 120; i++) {  // обнуление
      s21_set_bit_0(src, i);
    }
    s21_set_bits_from_int_to_decimal(power, src, 112);  // заполнение
  }
}

// для банковского округления --
// формирует  decimal = 0.5
s21_decimal s21_decimal_get_zerofive(void) {
  s21_decimal result = {{0x5, 0x0, 0x0, 0x10000}};

  return result;
}

int s21_decimal_even(s21_decimal value) { return (value.bits[0] & 1) != 1; }

// int s21_normalize(s21_decimal *num_1, s21_decimal *num_2) {
//   int power_num_1 = s21_get_power_of_decimal(*num_1);
//   int power_num_2 = s21_get_power_of_decimal(*num_2);
//   s21_set_power_of_decimal(num_1, 0);  // ставим степени чисел в 0
//   s21_set_power_of_decimal(num_2, 0);
//   s21_decimal bit_number_10 = {10, 0, 0, 0};

//   if (power_num_1 < power_num_2) {
//     while (power_num_2 - power_num_1) {
//       s21_mul(*num_1, bit_number_10, num_1);
//       power_num_1++;
//     }
//     return (power_num_1);
//   } else if (power_num_2 < power_num_1) {
//     while (power_num_1 - power_num_2) {
//       s21_mul(*num_2, bit_number_10, num_2);
//       power_num_2++;
//     }
//     return (power_num_2);
//   }
//   return (power_num_1);
// }

void s21_from_unsigned_long_int_to_decimal(unsigned long int src,
                                           s21_decimal *dst) {
  int exp = 0;
  src = fabs((double)src);
  while (src >= pow(2, exp)) exp++;
  while (src) {
    if (src - pow(2, exp) < 0) {
      s21_set_bit_0(dst, exp);
    } else {
      src -= pow(2, exp);
      s21_set_bit_1(dst, exp);
    }
    exp--;
  }
}

// int s21_rewrite_float_bits_to_buff(s21_decimal *buff,
//                                    float val) {  //  зачем написал ?? хз..
//   int float_in_32bit = *(int *)&val;
//   int index = 31;
//   long int i = two_32 / 2;  // 2^32
//   int scale = 0;

//   while (i != 1) {
//     if (float_in_32bit & i && i == two_32 / 2) {  // проверка знака числа
//       // set_sign_of_number(buff, (int)val);
//       s21_set_bit_1(buff, 31);
//       // мжно заменить просто на постановку бита в 31 бит
//     } else if ((float_in_32bit & i) &&
//                (index < 31 && index > 22)) {  // учет степени
//       scale += pow(2, index - 23);
//       s21_set_bit_1(buff, index);
//       // если index от 30 до 23 то это степень, относительно 127
//     } else if (float_in_32bit & i) {  // постановка бита в 1
//       s21_set_bit_1(buff, index);
//     } else {  // постановка бита в 0
//       s21_set_bit_0(buff, index);
//     }
//     i >>= 1;
//     index--;
//   }
//   return (scale -= 127);
// }

int get_float_exp_from_string(char *str, int *sign_of_float_power) {
  int result = 0;
  char *ptr = str;
  while (*ptr) {
    if (*ptr == 'E') {
      if (*(ptr + 1) == '+') {
        *sign_of_float_power = 1;
      } else if (*(ptr + 1) == '-') {
        *sign_of_float_power = 2;
      }
      ++ptr;
      result = strtol(ptr, NULL, 10);
      break;
    }
    ++ptr;
  }

  return result;
}

int s21_sign_handle(s21_decimal *value_1, s21_decimal *value_2,
                    s21_decimal *result, int function) {
  // function = 0 (from s21_add)
  // function = 1 (from s21_sub)
  int output = 0;
  int sign_1 = s21_get_bit(value_1, 127);
  int sign_2 = s21_get_bit(value_2, 127);

  // варианты для s21_sub
  if (function) {
    if (sign_1 == 1 && sign_2 == 0) {  // если пришло -3 - 2
      s21_set_bit_1(value_2, 127);
      output = s21_add(*value_1, *value_2, result);  // отправляем -3 + (-2)

    } else if ((sign_1 == 0 && sign_2 == 1)) {  // если пришло 3 - (-2)
      s21_set_bit_0(value_2, 127);
      output = s21_add(*value_1, *value_2, result);  // отправляем 3 + 2

    } else {
      output = 2;
    }
    // варианты для s21_add
  } else {
    if (sign_1 == 1 && sign_2 == 0) {  //  если пришло -3 + 2
      s21_set_bit_0(value_1, 127);
      output = s21_sub(*value_2, *value_1, result);  // отправляем 2 - 3

    } else if (sign_1 == 0 && sign_2 == 1) {  // если пришло 3 + (-2)
      s21_set_bit_0(value_2, 127);
      output = s21_sub(*value_1, *value_2, result);  // отправляем 3 - 2

    } else {  // остальные случаи обрабатываются в самих функциях
      output = 2;
    }
  }

  return output;
}
