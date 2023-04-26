#include "s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_decimal tmp_1 = value_1;
  s21_decimal tmp_2 = value_2;
  s21_decimal tmp_result = *result;
  s21_decimal reminder = {0};
  s21_decimal power_0 = {1, 0, 0, 0};
  s21_decimal power_1 = value_2;
  int power_of_value_2 = 0;
  int check_reminder = 0;

  // добавить нормализацию
  if (result) {
    s21_set_dec_number_to_0(&tmp_result);
    while (!check_reminder) {
      for (; s21_is_less_or_equal(tmp_2, tmp_1); power_of_value_2++) {
        shift_bit_left(&tmp_2, 1);
        // сдвигаем влево tmp_2 пока он <= tmp_1
      }
      shift_bit_right(&tmp_2, 1);  // сдвиг вправо, когда tmp_2 был меньше tmp_1
      power_of_value_2 -= 1;
      s21_sub(tmp_1, tmp_2, &tmp_1);
      tmp_2 = value_2;

      if (power_of_value_2 == 1) {
        s21_add(tmp_result, power_1, &tmp_result);
      } else if (power_of_value_2 == 0) {
        s21_add(tmp_result, power_0, &tmp_result);
      } else {
        shift_bit_left(&tmp_2, (power_of_value_2 - 1));
        s21_add(tmp_result, tmp_2, &tmp_result);
      }
      tmp_2 = value_2;
      power_of_value_2 = 0;
      reminder = tmp_1;
      check_reminder = s21_is_less(reminder, value_2);
    }
  }

  return (1);
}