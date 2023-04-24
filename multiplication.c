#include "s21_decimal.h"
// добавить проверку знака и переполнения
// добавлена tmp ошибки из-за неё только могут быть
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int output = CONVERSATION_ERROR;

  if (result) {
    int count_1 = 0, count_2 = 0;
    int index = 0;
    // int res_of_summ = 0; принимал s21_add
    s21_decimal step = {0};
    s21_set_dec_number_to_0(result);
    s21_decimal tmp = *result;

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
      while (index != 96) {
        if (s21_get_bit(&value_2, index)) {
          step = value_1;
          s21_shift_bits(&step, index);
          s21_add(step, tmp, &tmp);
          s21_set_dec_number_to_0(&step);
        }
        index++;
      }
    }
    output = CONVERSATION_OK;  // тут, до проверки на overflow
    *result = tmp;
  }
  return output;
}
