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

    не верно
    404440 / 4.4
    0.00000000000000000000000002 / 2 26 и дальше степени
*/

//------------------------Дление big_decimal----------------------//

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
    s21_set_big_dec_number_to_0(result);
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
            s21_sub_big(tmp_1, tmp_2, &tmp_1);
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
      // может быть переполнение
      if (!s21_is_decimal_0_big(reminder)) {
        while (s21_is_less_big(reminder, tmp_2) && !flg_end_of_95_bit) {
          flg_end_of_95_bit =
              s21_mul_decimal_by_10_big(&final_tmp_result);  // чекни
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

  return (output);  // переделай вывод и глянь s21_div_by_10
}