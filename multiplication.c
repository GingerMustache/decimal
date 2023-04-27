#include "s21_decimal.h"
// добавить проверку и переполнения
// добавлена tmp ошибки из-за неё только могут быть
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int output = CONVERSATION_ERROR;

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

    if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
      s21_normalize(&value_1, &value_2);  // нормализация
    }

    while (index != 63) {  // проверка что на что умножать будет
      count_1 += s21_get_bit(&value_1, index);
      count_2 += s21_get_bit(&value_2, index);
      index++;
    }
    index = 0;
    // case 1, где меньше едииц на то и умножаем
    if (count_1 < count_2) {
      while (index != 96) {
        if (s21_get_bit(&value_1, index)) {
          step = value_2;
          s21_shift_bits(&step, index);
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
          s21_add(step, tmp, &tmp);
          s21_set_dec_number_to_0(&step);
        }
        index++;
      }
    }
    if (output == CONVERSATION_OK) {
      // output = CONVERSATION_OK;  // тут, до проверки на overflow
      *result = tmp;
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
