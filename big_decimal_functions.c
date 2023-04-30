#include "s21_decimal.h"

// добавить обработку ошибок
// проверить заполнение периодом и добавить его обработку
// походу тут не нужна нормализация

int s21_sub_big(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal *result) {
  int output = CONVERSATION_ERROR;
  int sign_1 = s21_get_bit_big(&value_1, 127);
  int sign_2 = s21_get_bit_big(&value_2, 127);
  // output = s21_sign_handle(&value_1, &value_2, result, 1);

  // if (output == 2) {
  s21_set_bit_0_big(&value_1, 127);
  s21_set_bit_0_big(&value_2, 127);
  s21_big_decimal val_1 = {0};
  s21_big_decimal val_2 = {0};
  s21_big_decimal tmp = {0};
  int index_bit = 0;
  int i = 1;
  int flag_bit_min = 0;
  int bit_of_num_1 = 0;
  int bit_of_num_2 = 0;
  int power_of_result = 0;
  int power_of_1 = s21_get_power_of_big_decimal(value_1);
  int power_of_2 = s21_get_power_of_big_decimal(value_2);

  if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
    power_of_result = s21_normalize_big(&value_1, &value_2);  // нормализация
  }

  if (s21_is_greater_or_equal_big(
          value_1, value_2)) {  // поменя с is_greter на is_greater_or_equal
    val_1 = value_1;
    val_2 = value_2;
    if (sign_1 == 1) s21_set_bit_1_big(&tmp, 127);
  } else {
    val_1 = value_2;
    val_2 = value_1;
    if ((sign_2 == 0 && sign_1 == 0)) s21_set_bit_1_big(&tmp, 127);
    if ((sign_2 == 1 && sign_1 == 1)) s21_set_bit_0_big(&tmp, 127);
  }

  while (index_bit != 96) {
    bit_of_num_1 = s21_get_bit_big(&val_1, index_bit);
    bit_of_num_2 = s21_get_bit_big(&val_2, index_bit);

    if (!bit_of_num_1 && !bit_of_num_2) {  // оба бита = 0
      s21_set_bit_0_big(&tmp, index_bit);

    } else if (bit_of_num_1 && !bit_of_num_2) {  // бит первого = 1, второго = 0
      s21_set_bit_1_big(&tmp, index_bit);

    } else if (!bit_of_num_1 && bit_of_num_2) {  // бит первого = 0, второго = 0
      i = 1;  // с ее помощью смотрим следующии биты
      s21_set_bit_0_big(&tmp, index_bit);
      // цикл будет искать ближайший бит = 1
      while (!flag_bit_min) {
        if (s21_get_bit_big(&val_1, index_bit + i)) {
          s21_set_bit_0_big(&val_1, index_bit + i);
          flag_bit_min = 1;
        } else {
          s21_set_bit_1_big(&val_1, (index_bit + i));
          i++;
        }
      }
      s21_set_bit_1_big(&tmp, index_bit);
      flag_bit_min = 0;

    } else if (bit_of_num_1 && bit_of_num_2) {  // оба бита = 1
      s21_set_bit_0_big(&tmp, index_bit);
    }
    index_bit++;
  }
  s21_set_power_of_big_decimal(&tmp, power_of_result);
  *result = tmp;
  output = CONVERSATION_OK;
  // }

  return output;
}

int s21_div_big(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal *result) {
  int output = CONVERSATION_OK;

  if (!result) {
    output = CONVERSATION_ERROR;  // а что тут должно быть??
  } else if (s21_is_decimal_0_big(value_2)) {  // деление на 0
    output = CONVERSATION_DIV_ZERO;
  } else if (s21_is_decimal_0_big(value_1)) {
    output = CONVERSATION_OK;  // если первое число 0
    s21_set_big_dec_number_to_0(result);
  } else if (s21_is_equal_big(value_1, value_2)) {
    output = CONVERSATION_OK;  // если числа и их степени одинаковы
    s21_set_dec_number_to_1_big(result);
  } else {
    int flg_end_of_95_bit = 0;
    s21_big_decimal tmp_result = {0};
    s21_big_decimal final_tmp_result = {0};
    s21_big_decimal reminder = {1};
    int power_of_value_2 = 0;
    int check_reminder = 0;
    int power_of_result = 0;
    int power_of_1 = s21_get_power_of_big_decimal(value_1);
    int power_of_2 = s21_get_power_of_big_decimal(value_2);
    // if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
    //   power_of_result = s21_normalize(&value_1, &value_2);  // нормализация
    // }
    // if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
    //   s21_normalize(&value_1, &value_2);  // нормализация
    // }
    s21_set_power_of_big_decimal(&value_1, 0);
    s21_set_power_of_big_decimal(&value_2, 0);
    s21_big_decimal tmp_1 = value_1;
    s21_big_decimal tmp_2 = value_2;

    while (!s21_is_decimal_0_big(reminder) && power_of_result <= 20) {  // <=27
      s21_set_big_dec_number_to_0(&reminder);
      while (!check_reminder) {
        for (; s21_is_less_big(tmp_2, tmp_1); power_of_value_2++) {
          shift_big_bit_left(&tmp_2, 1, 0, 5);
          // сдвигаем влево tmp_2 пока он <= tmp_1
        }
        if (s21_is_greater_big(tmp_2, tmp_1)) {
          shift_bit_right_big(&tmp_2, 1, 0);
          power_of_value_2 -= 1;
        }
        if (power_of_value_2 < 0) {
          //   shift_bit_left(&tmp_2, 1);
          tmp_2 = value_2;
        } else {
          if (s21_is_less_or_equal_big(tmp_2, tmp_1)) {
            s21_sub_big(tmp_1, tmp_2, &tmp_1);  // надо переписать вычитание
            s21_set_bit_1_big(&tmp_result,
                              power_of_value_2);  // ставим бит степени
            tmp_2 = value_2;
          }
        }

        power_of_value_2 = 0;
        reminder = tmp_1;
        check_reminder = s21_is_less_big(reminder, value_2);
      }
      s21_big_add(final_tmp_result, tmp_result, &final_tmp_result);
      // может быть переполнение
      if (!s21_is_decimal_0_big(reminder)) {
        while (s21_is_less_big(reminder, tmp_2) && !flg_end_of_95_bit) {
          flg_end_of_95_bit = s21_mul_decimal_by_10_big(&final_tmp_result);
          power_of_result++;  // не правильно работает
          s21_mul_decimal_by_10_big(&reminder);
        }
        check_reminder = 0;
        s21_set_big_dec_number_to_0(&tmp_result);

        tmp_1 = reminder;
      }
    }
    // if (power_of_1 > power_of_2) {
    //   power_of_result -= power_of_1;  //??????? нужно больше тестов
    //   //   power_of_result /*был -*/ = power_of_1 - power_of_2;
    // } else if (power_of_1 < power_of_2) {
    //   power_of_result += power_of_1 - power_of_2;
    // }
    power_of_result += power_of_1 - power_of_2;
    while (power_of_result < 0) {
      s21_mul_decimal_by_10_big(  // надо проверять эту строку
          &final_tmp_result);  // надо добавить флаг, что при возвращении 1
      power_of_result++;
    }
    s21_set_power_of_big_decimal(&final_tmp_result, power_of_result);
    *result = final_tmp_result;
  }

  return (output);
}

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
  int i = 95;
  int output = 0;  // 0 - false, 1 - true
  int sign_1 = s21_get_bit_big(&num_1, 127);
  int sign_2 = s21_get_bit_big(&num_2, 127);
  int power_of_1 = s21_get_power_of_big_decimal(num_1);
  int power_of_2 = s21_get_power_of_big_decimal(num_2);

  if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
    s21_normalize_big(&num_1, &num_2);  // нормализация
  }
  if (sign_1 < sign_2) {
    output = 1;
  } else if (sign_1 > sign_2) {
    output = 0;
  } else {
    while (res_1 == res_2 && i != -1) {
      res_1 = s21_get_bit_big(&num_1, i);
      res_2 = s21_get_bit_big(&num_2, i);
      i--;
    }
    if (sign_1 == 0 && sign_2 == 0) {  // если числа положительные
      if (res_1 > res_2)
        output = 1;
      else
        output = 0;
    } else {  // если числа отрицательные
      if (res_1 < res_2)
        output = 1;
      else
        output = 0;
    }
  }

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
  int res = s21_big_mul(*num, decimal_10, num);
  printf("res of  = %d\n", res);
  if (!res)  // 0
    return (0);
  else
    return (1);
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
