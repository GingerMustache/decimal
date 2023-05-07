#include "s21_decimal.h"

// добавить обработку ошибок
// проверить заполнение периодом и добавить его обработку
// походу тут не нужна нормализация
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int output = CONVERSATION_OK;

  if (!result) {
    output = CONVERSATION_ERROR;  // а что тут должно быть??
  } else if (s21_is_decimal_0(value_2)) {  // деление на 0
    output = CONVERSATION_DIV_ZERO;
    *result = s21_decimal_get_inf();
  } else if (s21_is_decimal_0(value_1)) {
    output = CONVERSATION_OK;  // если первое число 0
    s21_set_dec_number_to_0(result);
  } else if (s21_is_equal(value_1, value_2)) {
    output = CONVERSATION_OK;  // если числа и их степени одинаковы
    s21_set_dec_number_to_1(result);
  } else {
    int flg_end_of_95_bit = 0;
    s21_decimal tmp_result = {0};
    s21_decimal final_tmp_result = {0};
    s21_decimal reminder = {1};
    int power_of_value_2 = 0;
    int check_reminder = 0;
    int power_of_result = 0;
    int power_of_1 = s21_get_power_of_decimal(value_1);
    int power_of_2 = s21_get_power_of_decimal(value_2);
    // if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
    //   power_of_result = s21_normalize(&value_1, &value_2);  // нормализация
    // }
    // if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
    //   s21_normalize(&value_1, &value_2);  // нормализация
    // }
    s21_set_power_of_decimal(&value_1, 0);
    s21_set_power_of_decimal(&value_2, 0);
    s21_decimal tmp_1 = value_1;
    s21_decimal tmp_2 = value_2;

    while (!s21_is_decimal_0(reminder) && power_of_result <= 27) {  // <=27
      s21_set_dec_number_to_0(&reminder);
      while (!check_reminder) {
        for (; s21_is_less(tmp_2, tmp_1); power_of_value_2++) {
          shift_bit_left(&tmp_2, 1);
          // сдвигаем влево tmp_2 пока он <= tmp_1
          // перепиши, как сделано в s21_div_decimal_by_10_big, а то не
          // прваильно работает на больших числах
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
      // может быть переполнение
      // тут можно отсечку сделать для транкейт
      if (!s21_is_decimal_0(reminder)) {
        while (s21_is_less(reminder, tmp_2) && !flg_end_of_95_bit) {
          flg_end_of_95_bit = s21_mul_decimal_by_10(&final_tmp_result);
          power_of_result++;  // не правильно работает
          s21_mul_decimal_by_10(&reminder);
        }
        check_reminder = 0;
        s21_set_dec_number_to_0(&tmp_result);

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
      s21_mul_decimal_by_10(  // надо проверять эту строку
          &final_tmp_result);  // надо добавить флаг, что при возвращении 1
      power_of_result++;
    }
    s21_set_power_of_decimal(&final_tmp_result, power_of_result);
    *result = final_tmp_result;
  }

  return (output);
}

/*
    считает верно
        8.24 / 2
        440 / 4.4 - конечная степень (-)
        100 / 10
        225.225 / 1.5
        0.02 / 0.5
        7 / 3
        404440 / 4.4
        0.00000000000000000000000002 / 2 26 и дальше степени
        0.00000000000000000002 / 2000000000 - ошибка SMALL
        0.003 / 11111111

    не верно
*/

//------------------------Дление big_decimal----------------------//

int s21_div_big(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal *result) {
  int output = CONVERSATION_OK;

  if (s21_is_decimal_0_big(value_2)) {  // деление на 0
    output = CONVERSATION_DIV_ZERO;
  } else if (s21_is_decimal_0_big(value_1)) {
    output = CONVERSATION_OK;  // если первое число 0
    s21_set_big_dec_number_to_0(result);
  } else if (s21_is_equal_big(value_1, value_2)) {
    output = CONVERSATION_OK;  // если числа и их степени одинаковы
    s21_set_dec_number_to_1_big(result);
  } else {
    s21_set_big_dec_number_to_0(result);
    // int flg_end_of_95_bit = 0;
    int rewrite = 3;
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
    s21_big_decimal prev_value = {0};
    s21_big_decimal _2 = {2, 0, 0, 0, 0, 0, 0};
    power_of_result += power_of_1 - power_of_2;  //  чекни если -

    while (!s21_is_decimal_0_big(reminder) && rewrite == 3) {  // <=27
      s21_set_big_dec_number_to_0(&reminder);
      while (!check_reminder) {
        for (; s21_is_less_big(tmp_2, tmp_1); power_of_value_2++) {
          if ((power_of_value_2 > 27 && power_of_value_2 < 31) ||
              (power_of_value_2 > 59 &&
               power_of_value_2 < 63) ||  // неправильно делает переход
              (power_of_value_2 > 91 && power_of_value_2 < 95) ||
              (power_of_value_2 > 123 && power_of_value_2 < 127) ||
              (power_of_value_2 > 155 && power_of_value_2 < 159) ||
              (power_of_value_2 > 187 && power_of_value_2 < 191)) {
            // может умножение на 2 пихнуть?
            prev_value = tmp_2;
            s21_big_mul(tmp_2, _2, &tmp_2, 0);
          } else {
            prev_value = tmp_2;
            shift_big_bit_left(&tmp_2, 1, 0, 5);
          }
          s21_print_big_decimal_number(&tmp_2);
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
            s21_sub_big(tmp_1, tmp_2, &tmp_1, 0);  // чекни потом 0 или 1 нужна
            s21_set_bit_1_big(&tmp_result,
                              power_of_value_2);  // ставим бит степени
            tmp_2 = value_2;
          }
        }

        power_of_value_2 = 0;
        reminder = tmp_1;
        check_reminder = s21_is_less_big(reminder, value_2);
      }
      s21_big_add(final_tmp_result, tmp_result, &final_tmp_result, 0);
      s21_print_big_decimal_number(&final_tmp_result);
      // может быть переполнение
      if (!s21_is_decimal_0_big(reminder)) {
        while (s21_is_less_big(reminder, tmp_2) &&
               rewrite == 3) {  // && !flg_end_of_95_bit
                                // flg_end_of_95_bit =
          tmp_result = final_tmp_result;
          s21_mul_decimal_by_10_big(&final_tmp_result);  // чекни
          rewrite = check_big_decimal(final_tmp_result);
          if (rewrite == 3 && power_of_result <= 27) {
            power_of_result++;
            s21_mul_decimal_by_10_big(&reminder);
          } else {
            final_tmp_result = tmp_result;
            rewrite = 0;
          }
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
    while (power_of_result < 0) {
      s21_mul_decimal_by_10_big(  // надо проверять эту строку
          &final_tmp_result);  // надо добавить флаг, что при возвращении 1
      power_of_result++;
    }
    if (power_of_result > 28) {
      output = CONVERSATION_SMALL;
    } else {
      s21_set_power_of_big_decimal(&final_tmp_result, power_of_result);
      *result = final_tmp_result;
    }
  }

  return (output);  // переделай вывод и глянь s21_div_by_10
}