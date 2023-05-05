#include "s21_decimal.h"
// добавить проверку на underflow
// добавлена tmp ошибки из-за неё только могут быть
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int output = CONVERSATION_OK;

  int count_1 = 0, count_2 = 0;
  int index = 0;
  int sign_1 = s21_get_bit(&value_1, 127);
  int sign_2 = s21_get_bit(&value_2, 127);
  s21_set_dec_number_to_0(result);
  // s21_decimal tmp = *result;
  // s21_decimal step = {0};
  // int power_of_result = 0;
  // степень пропадает при вычислениях и появляется *result = tmp
  // степепнь сохраняется  в нормализации
  // int power_of_1 = s21_get_power_of_decimal(value_1);
  // int power_of_2 = s21_get_power_of_decimal(value_2);
  // int res_of_summ = 0; принимал s21_add

  s21_set_bit_0(&value_2, 127);  // устанавливаем знаки в (+)
  s21_set_bit_0(&value_1, 127);

  // лучше сделать в биг_децимал
  // if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
  //   s21_normalize(&value_1, &value_2);  // нормализация
  // }

  while (index != 63) {  // проверка что на что умножать будет
    count_1 += s21_get_bit(&value_1, index);
    count_2 += s21_get_bit(&value_2, index);
    index++;
  }
  index = 0;

  s21_big_decimal big_value_1 = {0};
  s21_big_decimal big_value_2 = {0};
  s21_big_decimal big_result = {0};
  rewrite_decimal_to_big(&big_value_1, value_1);
  rewrite_decimal_to_big(&big_value_2, value_2);

  // можно учесть умножение на 0 сразу
  if (count_1 < count_2) {
    s21_big_mul(big_value_1, big_value_2, &big_result, 1);
  } else {
    s21_big_mul(big_value_2, big_value_1, &big_result, 1);
  }
  // case 1, где меньше едииц на то и умножаем
  //   if (count_1 < count_2) {
  //     while (index != 96 && output == 0) {
  //       if (s21_get_bit(&value_1, index)) {
  //         step = value_2;
  //         output = s21_shift_bits(&step, index);
  //         s21_add(step, tmp, &tmp);
  //         s21_set_dec_number_to_0(&step);
  //       }
  //       index++;
  //     }
  //   } else {
  //     while (index != 96 && output == 0) {
  //       if (s21_get_bit(&value_2, index)) {
  //         step = value_1;
  //         output = s21_shift_bits(&step, index);
  //         s21_add(step, tmp, &tmp);  // можно пристроить output сюда
  //         s21_set_dec_number_to_0(&step);
  //       }
  //       index++;
  //     }
  //   }
  //   *result = tmp;
  //   if (output == CONVERSATION_OK) {
  //     // output = CONVERSATION_OK;  // тут, до проверки на overflow
  //     // обрезает нули при необходимости
  //     if (power_of_1 && power_of_2) {
  //       s21_truncate_zero(result, abs(power_of_1 - power_of_2));
  //     }
  //     s21_set_power_of_decimal(result,
  //                              power_of_1 + power_of_2);  // постановка
  //                              степени
  if (sign_1 != sign_2) {  // постановка знака
    s21_set_bit_1(result, 127);
  } else {
    s21_set_bit_0(result, 127);
  }
  //   }
  // }
  return output;
}

//------------------------Умножение big_decimal----------------------//

// func - переменная показывающая откуда пришел вызов функции, если 1 из
// умножния, если нет то откуда угодно
int s21_big_mul(s21_big_decimal big_value_1, s21_big_decimal big_value_2,
                s21_big_decimal* result, int func) {
  int index = 0;
  int output = 1;  // поменял с end_mul
  s21_big_decimal step = {0};
  s21_set_big_dec_number_to_0(result);
  s21_big_decimal tmp = *result;
  s21_big_decimal garbage = {0};
  // s21_big_decimal big_10 = {10, 0, 0, 0, 0, 0, 0};  // не удаляq
  int count_1 = 0, count_2 = 0;
  int power_of_1 = s21_get_power_of_big_decimal(big_value_1);
  int power_of_2 = s21_get_power_of_big_decimal(big_value_2);
  int power_of_result = power_of_1 + power_of_2;
  // power больше 1, когда кличество значаящих единиц в двух меожителя
  // превосходи 95 бит
  int power = 0;
  // int flg_div = 1;

  s21_big_decimal fractional = {0};
  s21_big_decimal value_unsigned_truncated = {0};

  if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
    s21_normalize_big(&big_value_1, &big_value_2);  // нормализация
  }
  while (index != 63) {  // проверка что на что умножать будет
    count_1 += s21_get_bit_big(&big_value_1, index);
    count_2 += s21_get_bit_big(&big_value_2, index);
    index++;
  }
  index = 0;

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

  // if (sign_1 != sign_2) {        // постановка знака
  //   s21_set_bit_1_big(result, 223);
  // } else {
  //   s21_set_bit_0_big(result, 223);
  // }

  if (func == 1) {
    power = count_ones(big_value_1, big_value_2);
    // s21_print_big_decimal_number(&big_tmp);
    int rewrite = check_big_decimal(tmp);
    if (rewrite == 3) {
      *result = tmp;
      // нужна проверка на степень
      // убираем нули
      if (power_of_1 || power_of_2) {  // было &&
        s21_truncate_zero_big(result);
      }
    } else if (power_of_result) {
      while (power_of_result && rewrite != 3) {  // сменил ||, как в вычитании

        // s21_div_big(tmp, big_10, &tmp);
        s21_div_decimal_by_10_big(&tmp, &garbage);
        // s21_print_big_decimal_number(&tmp);
        // rewrite = check_big_decimal(tmp);
        // if (rewrite != 3) {
        // s21_round_big(tmp, &tmp);
        // замена round
        s21_truncate_big(tmp, &value_unsigned_truncated);
        s21_sub_big(tmp, value_unsigned_truncated, &fractional, 0);
        tmp = s21_round_banking_big(value_unsigned_truncated, fractional);

        // s21_print_big_decimal_number(&tmp);
        rewrite = check_big_decimal(tmp);
        // }
        power_of_result--;
        // flg_div = 0;
      }
      if (!power_of_result && rewrite != 3) {
        output = CONVERSATION_BIG;
      } else {
        output = CONVERSATION_OK;
        *result = tmp;
        if (power_of_1 || power_of_2) {  // было &&
          s21_truncate_zero_big(result);
          // s21_print_big_decimal_number(&tmp);
        }
      }
    } else {
      output = CONVERSATION_BIG;
    }
    // пока не понял надо ли
    // постановка степени
    if (power_of_1 + power_of_2 - power <= 28) {
      s21_set_power_of_big_decimal(result, power_of_1 + power_of_2 - power);
      // s21_set_power_of_big_decimal(result, power_of_result);
      output = CONVERSATION_OK;
    } else {
      // можно для деления что то подставить
      output = CONVERSATION_SMALL;
    }

  } else {
    *result = tmp;
  }
  return output;  // при заполнении всего биг_децимал вернет 0
}

// int count_ones(s21_big_decimal v_1, s21_big_decimal v_2) {
//   int count_sum = 0;
//   int v1_95 = 95;
//   int v2_95 = 95;
//   while (!s21_get_bit_big(&v_1, v1_95)) {
//     v1_95--;
//   }
//   while (!s21_get_bit_big(&v_2, v2_95)) {
//     v2_95--;
//   }
//   count_sum = v1_95 + v2_95;
//   if (count_sum >= 95)
//     return (count_sum - 95);
//   else
//     return (0);
// }

int count_ones(s21_big_decimal v_1, s21_big_decimal v_2) {
  int count_sum = 0;
  int count_1 = 0;
  int count_2 = 0;
  int v1_95 = 95;
  int v2_95 = 95;
  while (v1_95) {
    count_1 += s21_get_bit_big(&v_1, v1_95);
    v1_95--;
  }
  while (v2_95) {
    count_2 += s21_get_bit_big(&v_2, v2_95);
    v2_95--;
  }
  count_sum = count_1 + count_2;
  if (count_sum >= 96)
    return (count_sum - 96);
  else
    return (0);
}
/*
    работает
    1234.567 * 0.00000000000000000000000006 = 1001011010110100001110 и степень

    проверить
    95 * на нули о обычные числа
    дюбые переполнения

*/
