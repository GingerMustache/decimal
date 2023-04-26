#include "s21_decimal.h"

// int s21_div_with_fract_part(s21_decimal value_1, s21_decimal value_2,
//                             s21_decimal *result, int power_of_value_2,
//                             int check_reminder) {
//   s21_decimal tmp_1 = value_1;
//   s21_decimal tmp_2 = value_2;
//   s21_decimal tmp_result = *result;
//   s21_decimal reminder = {0};

//   while (!check_reminder) {
//     for (; s21_is_less_or_equal(tmp_2, tmp_1); power_of_value_2++) {
//       shift_bit_left(&tmp_2, 1);
//       // сдвигаем влево tmp_2 пока он <= tmp_1
//     }
//     shift_bit_right(&tmp_2,
//                     1);  // сдвиг вправо, когда tmp_2 был меньше tmp_1
//     power_of_value_2 -= 1;
//     s21_sub(tmp_1, tmp_2, &tmp_1);
//     s21_set_bit_1(&tmp_result, power_of_value_2);  // ставим бит степени
//     tmp_2 = value_2;

//     power_of_value_2 = 0;
//     reminder = tmp_1;
//     check_reminder = s21_is_less(reminder, value_2);
//   }
//   return (1);
// }

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_decimal tmp_1 = value_1;
  s21_decimal tmp_2 = value_2;
  s21_decimal tmp_result = {0};
  s21_decimal final_tmp_result = {0};
  s21_decimal reminder = {1};
  int power_of_value_2 = 0;
  int check_reminder = 0;

  // добавить нормализацию
  if (result) {
    while (!s21_is_decimal_0(reminder)) {  // || степень результата больше 28
      s21_set_dec_number_to_0(&reminder);
      while (!check_reminder) {
        for (; s21_is_less_or_equal(tmp_2, tmp_1); power_of_value_2++) {
          shift_bit_left(&tmp_2, 1);
          // сдвигаем влево tmp_2 пока он <= tmp_1
        }
        shift_bit_right(&tmp_2,
                        1);  // сдвиг вправо, когда tmp_2 был меньше tmp_1
        power_of_value_2 -= 1;
        s21_sub(tmp_1, tmp_2, &tmp_1);
        s21_set_bit_1(&tmp_result, power_of_value_2);  // ставим бит степени
        tmp_2 = value_2;

        power_of_value_2 = 0;
        reminder = tmp_1;
        check_reminder = s21_is_less(reminder, value_2);
      }
      s21_add(final_tmp_result, tmp_result, &final_tmp_result);

      s21_mul_decimal_by_10(&final_tmp_result);
      s21_mul_decimal_by_10(&reminder);  // убрать умножение последнее
      check_reminder = 0;
      s21_set_dec_number_to_0(&tmp_result);

      tmp_1 = reminder;
    }
  }

  return (1);
}