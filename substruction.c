#include "s21_decimal.h"

// функция для принятия решения (-) или (+)
int s21_sign_handle(s21_decimal *value_1, s21_decimal *value_2,
                    s21_decimal *result, int function) {
  // function = 0 (from s21_add)
  // function = 1 (from s21_sub)
  int output = 0;
  int sign_1 = s21_get_bit(value_1, 127);
  int sign_2 = s21_get_bit(value_2, 127);

  // варианты для s21_sub
  if (function) {
    if (sign_1 == 1 && sign_2 == 0) {  // если пришло -3 - 2
      s21_set_bit_1(value_2, 127);
      output = s21_add(*value_1, *value_2, result);  // отправляем -3 + (-2)

    } else if ((sign_1 == 0 && sign_2 == 1)) {  // если пришло 3 - (-2)
      s21_set_bit_0(value_2, 127);
      output = s21_add(*value_1, *value_2, result);  // отправляем 3 + 2

    } else {
      output = 2;
    }
    // варианты для s21_add
  } else {
    if (sign_1 == 1 && sign_2 == 0) {  //  если пришло -3 + 2
      s21_set_bit_0(value_1, 127);
      output = s21_sub(*value_2, *value_1, result);  // отправляем 2 - 3

    } else if (sign_1 == 0 && sign_2 == 1) {  // если пришло 3 + (-2)
      s21_set_bit_0(value_2, 127);
      output = s21_sub(*value_1, *value_2, result);  // отправляем 3 - 2

    } else {  // остальные случаи обрабатываются в самих функциях
      output = 2;
    }
  }

  return output;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int output = CONVERSATION_ERROR;
  int sign_1 = s21_get_bit(&value_1, 127);
  int sign_2 = s21_get_bit(&value_2, 127);
  output = s21_sign_handle(&value_1, &value_2, result, 1);

  if (!result) {
    output = CONVERSATION_ERROR;
  } else if (output == 2) {
    s21_set_bit_0(&value_1, 127);
    s21_set_bit_0(&value_2, 127);
    s21_decimal val_1 = {0};
    s21_decimal val_2 = {0};
    s21_decimal tmp = {0};
    int index_bit = 0;
    int i = 1;
    int flag_bit_min = 0;
    int bit_of_num_1 = 0;
    int bit_of_num_2 = 0;
    int power_of_result = 0;
    int power_of_1 = s21_get_power_of_decimal(value_1);
    int power_of_2 = s21_get_power_of_decimal(value_2);

    if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
      power_of_result = s21_normalize(&value_1, &value_2);  // нормализация
    }

    if (s21_is_greater(value_1, value_2)) {
      val_1 = value_1;
      val_2 = value_2;
      if (sign_1 == 1) s21_set_bit_1(&tmp, 127);
    } else {
      val_1 = value_2;
      val_2 = value_1;
      if ((sign_2 == 0 && sign_1 == 0)) s21_set_bit_1(&tmp, 127);
      if ((sign_2 == 1 && sign_1 == 1)) s21_set_bit_0(&tmp, 127);
    }

    while (index_bit != 96) {
      bit_of_num_1 = s21_get_bit(&val_1, index_bit);
      bit_of_num_2 = s21_get_bit(&val_2, index_bit);

      if (!bit_of_num_1 && !bit_of_num_2) {  // оба бита = 0
        s21_set_bit_0(&tmp, index_bit);

      } else if (bit_of_num_1 &&
                 !bit_of_num_2) {  // бит первого = 1, второго = 0
        s21_set_bit_1(&tmp, index_bit);

      } else if (!bit_of_num_1 &&
                 bit_of_num_2) {  // бит первого = 0, второго = 0
        i = 1;  // с ее помощью смотрим следующии биты
        s21_set_bit_0(&tmp, index_bit);
        // цикл будет искать ближайший бит = 1
        while (!flag_bit_min) {
          if (s21_get_bit(&val_1, index_bit + i)) {
            s21_set_bit_0(&val_1, index_bit + i);
            flag_bit_min = 1;
          } else {
            s21_set_bit_1(&val_1, (index_bit + i));
            i++;
          }
        }
        s21_set_bit_1(&tmp, index_bit);
        flag_bit_min = 0;

      } else if (bit_of_num_1 && bit_of_num_2) {  // оба бита = 1
        s21_set_bit_0(&tmp, index_bit);
      }
      index_bit++;
    }
    *result = tmp;
    s21_set_power_of_decimal(result, power_of_result);
    output = CONVERSATION_OK;
  }

  return output;
}