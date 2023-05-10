#include "s21_decimal.h"

// добавить обработку ошибок
// проверить заполнение периодом и добавить его обработку
// походу тут не нужна нормализация

int s21_normalize_big(s21_big_decimal *num_1, s21_big_decimal *num_2) {
  int power_num_1 = s21_get_power_of_big_decimal(*num_1);
  int power_num_2 = s21_get_power_of_big_decimal(*num_2);
  s21_set_power_of_big_decimal(num_1, 0);  // ставим степени чисел в 0
  s21_set_power_of_big_decimal(num_2, 0);
  s21_big_decimal bit_number_10 = {10, 0, 0, 0, 0, 0, 0};

  if (power_num_1 < power_num_2) {
    while (power_num_2 - power_num_1) {
      s21_big_mul(*num_1, bit_number_10, num_1, 0);
      power_num_1++;
    }
    // s21_print_big_decimal_number(num_1);
    return (power_num_1);
  } else if (power_num_2 < power_num_1) {
    while (power_num_1 - power_num_2) {
      s21_big_mul(*num_2, bit_number_10, num_2, 0);
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

void rewrite_from_big_decimal_to_decimal(s21_big_decimal big_decimal,
                                         s21_decimal *decimal) {
  int power_of_decimal = s21_get_power_of_big_decimal(big_decimal);

  for (int i = 0; i != 96; i++) {
    if (s21_get_bit_big(&big_decimal, i)) {
      s21_set_bit_1(decimal, i);
    } else {
      s21_set_bit_0(decimal, i);
    }
  }
  s21_set_power_of_decimal(decimal, power_of_decimal);
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

int s21_truncate_zero_big(s21_big_decimal *value) {
  int output = 0;
  s21_big_decimal reminder = {0};
  s21_big_decimal zero_num = {0};
  s21_big_decimal tmp = {0};
  if (s21_is_equal_big(*value, zero_num)) {
    output = 1;
  } else {
    while (s21_is_equal_big(reminder, zero_num)) {
      tmp = *value;
      s21_div_decimal_by_10_big(value, &reminder);
      output++;
    }
  }
  *value = tmp;
  return (output - 1);
}

int s21_is_equal_big(s21_big_decimal num_1, s21_big_decimal num_2) {
  int output = 0;
  int power_of_1 = s21_get_power_of_big_decimal(num_1);
  int power_of_2 = s21_get_power_of_big_decimal(num_2);

  if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
    s21_normalize_big(&num_1, &num_2);  // нормализация
  }

  output = num_1.bits[0] == num_2.bits[0] && num_1.bits[1] == num_2.bits[1] &&
           num_1.bits[2] == num_2.bits[2] && num_1.bits[3] == num_2.bits[3] &&
           num_1.bits[4] == num_2.bits[4] && num_1.bits[5] == num_2.bits[5] &&
           num_1.bits[6] == num_2.bits[6];

  return output;
}

int s21_is_decimal_0_big(s21_big_decimal dec_num) {
  s21_big_decimal zero_decimal = {0};
  return s21_is_equal_big(zero_decimal, dec_num);
}

void s21_set_dec_number_to_1_big(s21_big_decimal *src_num) {
  s21_big_decimal decimal_1 = {1};
  *src_num = decimal_1;
}

int s21_is_greater_big(s21_big_decimal num_1, s21_big_decimal num_2) {
  int res_1 = 0, res_2 = 0;
  int i = 191;
  int output = 0;  // 0 - false, 1 - true
  int power_of_1 = s21_get_power_of_big_decimal(num_1);
  int power_of_2 = s21_get_power_of_big_decimal(num_2);

  if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
    s21_normalize_big(&num_1, &num_2);  // нормализация
  }

  while (res_1 == res_2 && i != -1) {
    res_1 = s21_get_bit_big(&num_1, i);
    res_2 = s21_get_bit_big(&num_2, i);
    i--;
  }

  if (res_1 > res_2)
    output = 1;
  else if (res_1 == res_2)
    output = 2;
  else
    output = 0;

  return output;
}

int s21_is_less_big(s21_big_decimal num_1, s21_big_decimal num_2) {
  return (s21_is_greater_big(num_2, num_1));
}

int s21_is_less_or_equal_big(s21_big_decimal num_1, s21_big_decimal num_2) {
  return s21_is_less_big(num_1, num_2) || s21_is_equal_big(num_1, num_2);
}

int s21_is_greater_or_equal_big(s21_big_decimal num_1, s21_big_decimal num_2) {
  return s21_is_greater_big(num_1, num_2) || s21_is_equal_big(num_1, num_2);
}

int s21_mul_decimal_by_10_big(s21_big_decimal *num) {
  s21_big_decimal decimal_10 = {10, 0, 0, 0, 0, 0, 0};
  int res =
      s21_big_mul(*num, decimal_10, num,
                  0);  // переписываю умнлжение, надо чекнуть потом условие
  // printf("res of  = %d\n", res);
  if (!res)  // 0
    return (0);
  else
    return (1);
}

// прообраз деления,но с остатком
int s21_div_decimal_by_10_big(s21_big_decimal *value_1,
                              s21_big_decimal *out_reminder) {
  s21_big_decimal value_2 = {10, 0, 0, 0, 0, 0, 0};
  s21_big_decimal _2 = {2, 0, 0, 0, 0, 0, 0};
  s21_big_decimal prev_value = {0};

  s21_big_decimal tmp_result = {0};
  s21_big_decimal final_tmp_result = {0};
  s21_big_decimal reminder = {1};
  int power_of_value_2 = 0;
  int check_reminder = 0;
  int power_of_result = 0;
  int power_of_1 = s21_get_power_of_big_decimal(*value_1);
  int power_of_2 = s21_get_power_of_big_decimal(value_2);

  s21_set_power_of_big_decimal(value_1, 0);
  s21_set_power_of_big_decimal(&value_2, 0);
  s21_big_decimal tmp_1 = *value_1;
  s21_big_decimal tmp_2 = value_2;

  s21_set_big_dec_number_to_0(&reminder);
  while (!check_reminder) {
    for (; s21_is_less_big(tmp_2, tmp_1); power_of_value_2++) {
      if ((power_of_value_2 > 27 && power_of_value_2 < 31) ||
          (power_of_value_2 > 59 && power_of_value_2 < 63) ||
          (power_of_value_2 > 91 && power_of_value_2 < 95) ||
          (power_of_value_2 > 123 && power_of_value_2 < 127) ||
          (power_of_value_2 > 155 && power_of_value_2 < 159) ||
          (power_of_value_2 > 187 && power_of_value_2 < 191)) {
        prev_value = tmp_2;
        s21_big_mul(tmp_2, _2, &tmp_2, 0);
      } else {
        prev_value = tmp_2;
        shift_big_bit_left(&tmp_2, 1, 0, 5);
      }
      // s21_print_big_decimal_number(&tmp_1);
      // s21_print_big_decimal_number(&tmp_2);
      // сдвигаем влево tmp_2 пока он <= tmp_1
    }
    if (s21_is_greater_big(tmp_2, tmp_1)) {
      if (power_of_value_2 > 27) {
        tmp_2 = prev_value;
      } else {
        shift_bit_right_big(&tmp_2, 1, 0);
      }
      power_of_value_2 -= 1;
    }
    if (power_of_value_2 < 0) {
      //   shift_bit_left(&tmp_2, 1);
      tmp_2 = value_2;
    } else {
      if (s21_is_less_or_equal_big(tmp_2, tmp_1)) {
        s21_sub_big(tmp_1, tmp_2, &tmp_1, 0);
        s21_set_bit_1_big(&tmp_result,
                          power_of_value_2);  // ставим бит степени
        tmp_2 = value_2;
      }
    }
    // s21_print_big_decimal_number(&tmp_result);
    power_of_value_2 = 0;
    reminder = tmp_1;
    check_reminder = s21_is_less_big(reminder, value_2);
    *out_reminder = reminder;
  }
  s21_big_add(final_tmp_result, tmp_result, &final_tmp_result, 0);
  // s21_print_big_decimal_number(&tmp_result);

  power_of_result += power_of_1 - power_of_2;
  while (power_of_result < 0) {
    s21_mul_decimal_by_10_big(&final_tmp_result);
    power_of_result++;
  }
  s21_set_power_of_big_decimal(&final_tmp_result, power_of_result);
  *value_1 = final_tmp_result;
  return (power_of_result);
}

int s21_round_big(s21_big_decimal value, s21_big_decimal *result) {
  int output = CONVERSATION_OK;
  s21_set_big_dec_number_to_0(result);
  // int sign = s21_decimal_get_sign(value);
  int sign = s21_get_bit_big(&value, 223);
  s21_big_decimal fractional = {0};
  s21_big_decimal value_unsigned_truncated = {0};
  // Убираем знак
  s21_big_decimal value_unsigned = value;
  s21_set_bit_0_big(&value_unsigned, 223);
  s21_truncate_big(value_unsigned, &value_unsigned_truncated);
  // s21_print_big_decimal_number(&value_unsigned_truncated);
  s21_sub_big(value_unsigned, value_unsigned_truncated, &fractional, 0);
  // s21_print_big_decimal_number(&fractional);
  value_unsigned_truncated =
      s21_round_banking_big(value_unsigned_truncated, fractional);

  *result = value_unsigned_truncated;
  // s21_decimal_set_sign(result, sign);
  if (sign) {  // постановка знака
    s21_set_bit_1_big(result, 223);
  } else {
    s21_set_bit_0_big(result, 223);
  }

  return output;
}

s21_big_decimal s21_round_banking_big(s21_big_decimal integral,
                                      s21_big_decimal fractional) {
  s21_big_decimal zerofive = s21_decimal_get_zerofive_big();
  s21_big_decimal result;
  s21_big_decimal decimal_one = {1};

  if (s21_is_equal_big(fractional, zerofive)) {
    if (s21_decimal_even_big(integral)) {
      result = integral;
    } else {
      s21_big_add(integral, decimal_one, &result, 0);
    }
  } else if (s21_is_greater_big(fractional, zerofive)) {
    s21_big_add(integral, decimal_one, &result, 0);
  } else {
    result = integral;
  }

  return result;
}

int s21_truncate_big(s21_big_decimal value, s21_big_decimal *result) {
  int output = CONVERSATION_ERROR;
  int power = 0;
  s21_big_decimal reminder = {0};
  if (result) {
    output = CONVERSATION_OK;
    power = s21_div_decimal_by_10_big(&value, &reminder);
    if (power) {
      while (power - 1) {
        s21_div_decimal_by_10_big(&value, &reminder);
        power--;
      }
    }
    s21_set_power_of_big_decimal(&value, 0);
    *result = value;
  }
  return output;
}

s21_big_decimal s21_decimal_get_zerofive_big(void) {
  s21_big_decimal result = {{0x5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x10000}};

  return result;
}

int s21_decimal_even_big(s21_big_decimal value) {
  return (value.bits[0] & 1) != 1;
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
