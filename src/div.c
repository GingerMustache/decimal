#include "s21_decimal.h"

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int output = CONVERSATION_OK;

  if (s21_is_decimal_0(value_2)) {  // деление на 0
    output = CONVERSATION_DIV_ZERO;
    s21_set_dec_number_to_0(result);
  } else if (s21_is_decimal_0(value_1)) {
    output = CONVERSATION_OK;  // если первое число 0
    s21_set_dec_number_to_0(result);
  } else if (s21_is_equal(value_1, value_2)) {
    output = CONVERSATION_OK;  // если числа и их степени одинаковы
    s21_set_dec_number_to_1(result);
  } else {
    s21_set_dec_number_to_0(result);
    // берем знаки
    int sign_1 = s21_get_bit(&value_1, 127);
    int sign_2 = s21_get_bit(&value_2, 127);

    int result_of_big_decimal = 0;
    s21_big_decimal big_value_1 = {0};
    s21_big_decimal big_value_2 = {0};
    s21_big_decimal big_result = {0};
    rewrite_decimal_to_big(&big_value_1, value_1);
    rewrite_decimal_to_big(&big_value_2, value_2);

    s21_set_bit_0(&value_2, 127);  // устанавливаем знаки в (+)
    s21_set_bit_0(&value_1, 127);
    // остальные случаи
    result_of_big_decimal = s21_div_big(big_value_1, big_value_2, &big_result);
    if (!result_of_big_decimal) {
      rewrite_from_big_decimal_to_decimal(big_result, result);
    } else if (result_of_big_decimal == 1) {
      output = CONVERSATION_BIG;
    } else if (result_of_big_decimal == 2) {
      output = CONVERSATION_SMALL;
    }

    if (sign_1 != sign_2) {  // постановка знака
      s21_set_bit_1(result, 127);
    } else {
      s21_set_bit_0(result, 127);
    }
  }

  return (output);
}

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
    int rewrite = 3;
    s21_big_decimal tmp_result = {0};
    s21_big_decimal final_tmp_result = {0};
    s21_big_decimal reminder = {{1, 0, 0, 0, 0, 0, 0}};
    int power_of_value_2 = 0;
    int check_reminder = 0;
    int power_of_result = 0;
    int power_of_1 = s21_get_power_of_big_decimal(value_1);
    int power_of_2 = s21_get_power_of_big_decimal(value_2);

    s21_set_power_of_big_decimal(&value_1, 0);
    s21_set_power_of_big_decimal(&value_2, 0);
    s21_big_decimal tmp_1 = value_1;
    s21_big_decimal tmp_2 = value_2;
    s21_big_decimal prev_value = {0};
    s21_big_decimal _2 = {{2, 0, 0, 0, 0, 0, 0}};

    power_of_result += power_of_1 - power_of_2;

    while (!s21_is_decimal_0_big(reminder) && rewrite == 3) {
      s21_set_big_dec_number_to_0(&reminder);
      while (!check_reminder) {
        for (; s21_is_less_big(tmp_2, tmp_1); power_of_value_2++) {
          if ((s21_get_bit_big(&tmp_2, 31)) || (s21_get_bit_big(&tmp_2, 63)) ||
              (s21_get_bit_big(&tmp_2, 95)) || (s21_get_bit_big(&tmp_2, 127)) ||
              (s21_get_bit_big(&tmp_2, 159))) {
            prev_value = tmp_2;
            s21_big_mul(tmp_2, _2, &tmp_2, 0);
          } else {
            prev_value = tmp_2;
            shift_big_bit_left(&tmp_2, 1, 0, 5);
          }
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
          tmp_2 = value_2;
        } else {
          if (s21_is_less_or_equal_big(tmp_2, tmp_1)) {
            s21_sub_big(tmp_1, tmp_2, &tmp_1, 0);
            s21_set_bit_1_big(&tmp_result, power_of_value_2);
            tmp_2 = value_2;
          }
        }

        power_of_value_2 = 0;
        reminder = tmp_1;
        check_reminder = s21_is_less_big(reminder, value_2);
      }
      s21_big_add(final_tmp_result, tmp_result, &final_tmp_result, 0);
      if (!s21_is_decimal_0_big(reminder)) {
        while (s21_is_less_big(reminder, tmp_2) == 1 && rewrite == 3) {
          tmp_result = final_tmp_result;
          s21_mul_decimal_by_10_big(&final_tmp_result);
          rewrite = check_big_decimal(final_tmp_result);
          if (rewrite == 3) {
            power_of_result++;
            s21_mul_decimal_by_10_big(&reminder);
          } else {
            final_tmp_result = tmp_result;
            s21_big_decimal garbege = {0};
            while (power_of_result > 28) {
              s21_div_decimal_by_10_big(&final_tmp_result, &garbege);
              power_of_result--;
              rewrite = check_big_decimal(final_tmp_result);
              if (rewrite != 3 && power_of_result == 28) {
                output = CONVERSATION_SMALL;
              }
            }
            rewrite = 0;
          }
        }
        check_reminder = 0;
        s21_set_big_dec_number_to_0(&tmp_result);

        tmp_1 = reminder;
      }
    }

    while (power_of_result < 0) {
      s21_mul_decimal_by_10_big(&final_tmp_result);
      power_of_result++;
    }
    rewrite = check_big_decimal(final_tmp_result);
    if (rewrite != 3 && power_of_result == 0) {
      return (output = CONVERSATION_BIG);
    }
    if (output == CONVERSATION_OK && power_of_result <= 28) {
      s21_set_power_of_big_decimal(&final_tmp_result, power_of_result);
      *result = final_tmp_result;
    } else {
      output = CONVERSATION_SMALL;
    }
  }

  return (output);
}
