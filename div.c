#include "s21_decimal.h"

// добавить обработку ошибок
// проверить заполнение периодом и добавить его обработку
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int output = CONVERSATION_OK;

  s21_decimal tmp_result = {0};
  s21_decimal final_tmp_result = {0};
  s21_decimal reminder = {1};
  int power_of_value_2 = 0;
  int check_reminder = 0;
  int power_of_result = 0;
  int power_of_1 = s21_get_power_of_decimal(value_1);
  int power_of_2 = s21_get_power_of_decimal(value_2);

  if (s21_is_decimal_0(value_2)) {  // деление на 0
    output = CONVERSATION_DIV_ZERO;
    *result = s21_decimal_get_inf();
  } else if (s21_is_decimal_0(value_1)) {
    output = CONVERSATION_OK;
    s21_set_dec_number_to_0(result);
  } else {
    if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
      power_of_result = s21_normalize(&value_1, &value_2);  // нормализация
    }
    s21_decimal tmp_1 = value_1;
    s21_decimal tmp_2 = value_2;

    while (!s21_is_decimal_0(reminder) && power_of_result <= 27) {
      s21_set_dec_number_to_0(&reminder);
      while (!check_reminder) {
        for (; s21_is_less(tmp_2, tmp_1); power_of_value_2++) {
          shift_bit_left(&tmp_2, 1);
          // сдвигаем влево tmp_2 пока он <= tmp_1
        }
        if (s21_is_greater(tmp_2, tmp_1)) {
          shift_bit_right(&tmp_2, 1);
          power_of_value_2 -= 1;
        }
        if (power_of_value_2 < 0) {
          //   shift_bit_left(&tmp_2, 1);
          tmp_2 = value_2;
        } else {
          if (s21_is_less_or_equal(tmp_2, tmp_1)) {
            s21_sub(tmp_1, tmp_2, &tmp_1);
            s21_set_bit_1(&tmp_result,
                          power_of_value_2);  // ставим бит степени
            tmp_2 = value_2;
          }
        }

        power_of_value_2 = 0;
        reminder = tmp_1;
        check_reminder = s21_is_less(reminder, value_2);
      }
      s21_add(final_tmp_result, tmp_result, &final_tmp_result);

      if (!s21_is_decimal_0(reminder)) {
        while (s21_is_less(reminder, tmp_2)) {
          s21_mul_decimal_by_10(&final_tmp_result);
          power_of_result++;  // не правильно работает
          s21_mul_decimal_by_10(&reminder);
        }
        check_reminder = 0;
        s21_set_dec_number_to_0(&tmp_result);

        tmp_1 = reminder;
      }
    }
    if (power_of_1 > power_of_2) {
      power_of_result -= power_of_1;  //??????? нужно больше тестов
      //   power_of_result /*был -*/ = power_of_1 - power_of_2;
    } else if (power_of_1 < power_of_2) {
      power_of_result += power_of_1 - power_of_2;
    }
    // может поставить,если степень > 28 = степень = 28?
    s21_set_power_of_decimal(&final_tmp_result, power_of_result);
    *result = final_tmp_result;
  }

  return (output);
}

/*
    считает верно
        8.24 / 2
        440 / 4.4
        100 / 10
        225.225 / 1.5
        0.02 / 0.5
        7 / 3 ?

    не верно
    0.00000000002 / 0.00000000002 при делении одинаковых чисел надо убирать
   степень
*/