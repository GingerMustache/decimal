#include "s21_decimal.h"
// добавить проверку на underflow
// добавлена tmp ошибки из-за неё только могут быть
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int output = CONVERSATION_OK;

  if (result) {
    int count_1 = 0, count_2 = 0;
    int index = 0;
    int sign_1 = s21_get_bit(&value_1, 127);
    int sign_2 = s21_get_bit(&value_2, 127);
    s21_decimal step = {0};
    s21_set_dec_number_to_0(result);
    s21_decimal tmp = *result;
    // int power_of_result = 0;
    // степень пропадает при вычислениях и появляется *result = tmp
    // степепнь сохраняется  в нормализации
    int power_of_1 = s21_get_power_of_decimal(value_1);
    int power_of_2 = s21_get_power_of_decimal(value_2);
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

    if (count_1 < count_2) {
      s21_big_mul(big_value_1, big_value_2, &big_result);
    } else {
      s21_big_mul(big_value_2, big_value_1, &big_result);
    }
    // case 1, где меньше едииц на то и умножаем
    if (count_1 < count_2) {
      while (index != 96 && output == 0) {
        if (s21_get_bit(&value_1, index)) {
          step = value_2;
          output = s21_shift_bits(&step, index);
          s21_add(step, tmp, &tmp);
          s21_set_dec_number_to_0(&step);
        }
        index++;
      }
    } else {
      while (index != 96 && output == 0) {
        if (s21_get_bit(&value_2, index)) {
          step = value_1;
          output = s21_shift_bits(&step, index);
          s21_add(step, tmp, &tmp);  // можно пристроить output сюда
          s21_set_dec_number_to_0(&step);
        }
        index++;
      }
    }
    *result = tmp;
    if (output == CONVERSATION_OK) {
      // output = CONVERSATION_OK;  // тут, до проверки на overflow
      // обрезает нули при необходимости
      if (power_of_1 && power_of_2) {
        s21_truncate_zero(result, abs(power_of_1 - power_of_2));
      }
      s21_set_power_of_decimal(result,
                               power_of_1 + power_of_2);  // постановка степени
      if (sign_1 != sign_2) {  // постановка знака
        s21_set_bit_1(result, 127);
      } else {
        s21_set_bit_0(result, 127);
      }
    }
  }
  return output;
}

//------------------------Умножение big_decimal----------------------//

int s21_big_mul(s21_big_decimal big_value_1, s21_big_decimal big_value_2,
                s21_big_decimal* result) {
  int index = 0;
  int end_mul = 1;
  s21_big_decimal step = {0};
  s21_set_big_dec_number_to_0(result);
  s21_big_decimal tmp = *result;

  int count_1 = 0, count_2 = 0;
  // int power_of_result = 0;
  // степень пропадает при вычислениях и появляется *result = tmp
  // степепнь сохраняется  в нормализации
  int power_of_1 = s21_get_power_of_big_decimal(big_value_1);
  int power_of_2 = s21_get_power_of_big_decimal(big_value_2);

  if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
    s21_normalize_big(&big_value_1, &big_value_2);  // нормализация
  }

  while (index != 63) {  // проверка что на что умножать будет
    count_1 += s21_get_bit_big(&big_value_1, index);
    count_2 += s21_get_bit_big(&big_value_2, index);
    index++;
  }
  index = 0;

  if (count_1 < count_2) {  // <
    while (index != 192 && end_mul) {
      if (s21_get_bit_big(&big_value_1, index)) {
        step = big_value_2;
        end_mul = s21_shift_bits_big(&step, index);
        s21_big_add(step, tmp, &tmp, 0);
        s21_set_big_dec_number_to_0(&step);
      }
      index++;
    }
  } else {
    while (index != 192 && end_mul) {
      if (s21_get_bit_big(&big_value_2, index)) {
        step = big_value_1;
        end_mul = s21_shift_bits_big(&step, index);
        s21_big_add(step, tmp, &tmp, 0);
        s21_set_big_dec_number_to_0(&step);
      }
      index++;
    }
  }

  *result = tmp;

  // обрезает нули при необходимости
  // // добавь в функцию нормальное деление
  if (power_of_1 && power_of_2) {
    s21_truncate_zero_big(result, abs(power_of_1 - power_of_2));
  }

  s21_set_power_of_big_decimal(result,
                               power_of_1 + power_of_2);  // постановка степени
  // if (sign_1 != sign_2) {        // постановка знака
  //   s21_set_bit_1_big(result, 223);
  // } else {
  //   s21_set_bit_0_big(result, 223);
  // }
  return end_mul;  // при заполнении всего биг_децимал вернет 0
}