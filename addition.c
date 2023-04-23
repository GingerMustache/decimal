#include "s21_decimal.h"

// add checks of overflow
// добавить проверку знака
// добавить normalize
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int output = CONVERSATION_ERROR;
  int sign_1 = s21_get_bit(&value_1, 127);
  // int sign_2 = s21_get_bit(&value_2, 127);
  output = sign_handle(&value_1, &value_2, result, 0);

  if (!result) {
    output = CONVERSATION_ERROR;
  } else if (output == 2) {
    int index = 0;
    char flag_bit_1 = 0;
    int num_1 = 0;
    int num_2 = 0;
    s21_decimal tmp = {0};

    if (sign_1) s21_set_bit_1(&tmp, 127);  // постановка знака чисел

    while (index != 96) {
      num_1 = s21_get_bit(&value_1, index);
      num_2 = s21_get_bit(&value_2, index);

      if ((num_1 && !num_2) || (!num_1 && num_2)) {
        if (flag_bit_1) {
          s21_set_bit_0(&tmp, index);
          flag_bit_1 = 1;
        } else {
          s21_set_bit_1(&tmp, index);
        }
      } else if (!num_1 && !num_2) {
        if (flag_bit_1) {
          s21_set_bit_1(&tmp, index);
          flag_bit_1 = 0;
        } else {
          s21_set_bit_0(&tmp, index);
        }
      } else {  // (num_1 && num_2)
        if (flag_bit_1) {
          s21_set_bit_1(&tmp, index);
          flag_bit_1 = 1;
        } else {
          s21_set_bit_0(&tmp, index);
          flag_bit_1 = 1;
        }
      }
      index++;
    }
    *result = tmp;
    output = CONVERSATION_OK;
  }

  return output;
}