#include "s21_decimal.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_decimal val_1 = {0};
  s21_decimal val_2 = {0};
  // int sign_flag = 0;
  int output = 0;
  int index = 0;
  int i = 1;
  int flag_bit_plus = 0;  // is it nessesary?
  int flag_bit_min = 0;
  int num_1 = 0;
  int num_2 = 0;
  s21_decimal tmp = {0};

  if (s21_is_greater(value_1, value_2)) {
    val_1 = value_1;
    val_2 = value_2;
  } else {
    val_1 = value_2;
    val_2 = value_1;
    // sign_flag = 1;
  }

  while (index != 96) {
    num_1 = s21_get_bit(&val_1, index);
    num_2 = s21_get_bit(&val_2, index);

    if (!num_1 && !num_2) {
      if (flag_bit_plus) {
        s21_set_bit_1(&tmp, index);
        flag_bit_plus = 0;
      } else {
        s21_set_bit_0(&tmp, index);
      }
    } else if (num_1 && !num_2) {
      s21_set_bit_1(&tmp, index);

    } else if (!num_1 && num_2) {
      i = 1;
      if (flag_bit_plus) {
        s21_set_bit_0(&tmp, index);
      } else {
        while (!flag_bit_min) {
          num_1 = s21_get_bit(&val_1, index + i);
          if (num_1) {
            s21_set_bit_0(
                &val_1,
                index +
                    i);  ///// what if index + i == 95 and no one bit is not 1??
            flag_bit_min = 1;
          } else {
            s21_set_bit_1(&val_1, (index + i));
            i++;
          }
        }
        s21_set_bit_1(&tmp, index);
        flag_bit_min = 0;
      }
    } else if (num_1 && num_2) {
      s21_set_bit_0(&tmp, index);
    }
    index++;
  }
  *result = tmp;
  // if (sign_flag) set_sign_of_number(result, -1); //Не понятно пока

  return output;
}