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
  // int sign_1 = s21_get_bit(&value_1, 127);
  // int sign_2 = s21_get_bit(&value_2, 127);
  output = s21_sign_handle(&value_1, &value_2, result, 1);

  if (output == 2) {
    // s21_decimal val_1 = {0};
    // s21_decimal val_2 = {0};
    s21_decimal tmp = {0};

    int big_decimal_output = 0;
    s21_big_decimal big_value_1 = {0};
    s21_big_decimal big_value_2 = {0};
    s21_big_decimal big_result = {0};
    rewrite_decimal_to_big(&big_value_1, value_1);
    rewrite_decimal_to_big(&big_value_2, value_2);

    // обработку знака лучше в decimal сделать
    big_decimal_output = s21_sub_big(big_value_1, big_value_2, &big_result, 1);
    if (!big_decimal_output) {
      rewrite_from_big_decimal_to_decimal(big_result, &tmp);
      if (s21_get_bit_big(&big_result, 223)) {
        s21_set_bit_1(&tmp, 127);
      }
      *result = tmp;
      output = CONVERSATION_OK;
    } else if (big_decimal_output == 1) {
      output = CONVERSATION_BIG;
    } else if (big_decimal_output == 2) {
      output = CONVERSATION_SMALL;
    }
    // int index_bit = 0;
    // int i = 1;
    // int flag_bit_min = 0;
    // int bit_of_num_1 = 0;
    // int bit_of_num_2 = 0;
    // int power_of_result = 0;
    // int power_of_1 = s21_get_power_of_decimal(value_1);
    // int power_of_2 = s21_get_power_of_decimal(value_2);
    // int big_decimal_output = 0;

    // if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
    //   power_of_result = s21_normalize(&value_1, &value_2);  // нормализация
    //   переехала в big_decimal
    // }
    // поменя с is_greter на is_greater_or_equal
    // if (s21_is_greater_or_equal(value_1, value_2)) {
    //   val_1 = value_1;
    //   val_2 = value_2;
    //   if (sign_1 == 1) s21_set_bit_1(&tmp, 127);
    // } else {
    //   val_1 = value_2;
    //   val_2 = value_1;
    //   if ((sign_2 == 0 && sign_1 == 0)) s21_set_bit_1(&tmp, 127);
    //   if ((sign_2 == 1 && sign_1 == 1)) s21_set_bit_0(&tmp, 127);
    // }

    // s21_big_decimal big_value_1 = {0};
    // s21_big_decimal big_value_2 = {0};
    // s21_big_decimal big_result = {0};
    // rewrite_decimal_to_big(&big_value_1, value_1);
    // rewrite_decimal_to_big(&big_value_2, value_2);

    // big_decimal_output = s21_big_add(big_value_1, big_value_2, &big_result,
    // 1);
    // (void)val_1;
    // (void)val_2;

    //   while (index_bit != 96) {
    //     bit_of_num_1 = s21_get_bit(&val_1, index_bit);
    //     bit_of_num_2 = s21_get_bit(&val_2, index_bit);

    //     if (!bit_of_num_1 && !bit_of_num_2) {  // оба бита = 0
    //       s21_set_bit_0(&tmp, index_bit);

    //     } else if (bit_of_num_1 &&
    //                !bit_of_num_2) {  // бит первого = 1, второго = 0
    //       s21_set_bit_1(&tmp, index_bit);

    //     } else if (!bit_of_num_1 &&
    //                bit_of_num_2) {  // бит первого = 0, второго = 0
    //       i = 1;  // с ее помощью смотрим следующии биты
    //       s21_set_bit_0(&tmp, index_bit);
    //       // цикл будет искать ближайший бит = 1
    //       while (!flag_bit_min) {
    //         if (s21_get_bit(&val_1, index_bit + i)) {
    //           s21_set_bit_0(&val_1, index_bit + i);
    //           flag_bit_min = 1;
    //         } else {
    //           s21_set_bit_1(&val_1, (index_bit + i));
    //           i++;
    //         }
    //       }
    //       s21_set_bit_1(&tmp, index_bit);
    //       flag_bit_min = 0;

    //     } else if (bit_of_num_1 && bit_of_num_2) {  // оба бита = 1
    //       s21_set_bit_0(&tmp, index_bit);
    //     }
    //     index_bit++;
    //   }
    //   s21_set_power_of_decimal(&tmp, power_of_result);
    //   *result = tmp;
    //   output = CONVERSATION_OK;
  }
  return output;
}

//------------------------Вычитание big_decimal----------------------//
// func - переменная показывающая необходимо ли использовать функционал
// переполнения
int s21_sub_big(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal *result, int func) {
  int output = CONVERSATION_OK;

  int sign_1 = s21_get_bit_big(&value_1, 223);
  int sign_2 = s21_get_bit_big(&value_2, 223);
  int sign_res = 0;

  s21_set_bit_0_big(&value_1, 223);
  s21_set_bit_0_big(&value_2, 223);
  s21_big_decimal val_1 = {0};
  s21_big_decimal val_2 = {0};
  s21_big_decimal tmp = {0};
  s21_big_decimal big_10 = {10, 0, 0, 0, 0, 0, 0};

  s21_big_decimal fractional = {0};
  s21_big_decimal value_unsigned_truncated = {0};

  int index_bit = 0;
  int i = 1;
  int flag_bit_min = 0;
  int bit_of_num_1 = 0;
  int bit_of_num_2 = 0;
  int power_of_result = 0;
  int power_of_1 = s21_get_power_of_big_decimal(value_1);
  int power_of_2 = s21_get_power_of_big_decimal(value_2);

  if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
    power_of_result = s21_normalize_big(&value_1, &value_2);  // нормализация
  }

  if (s21_is_greater_or_equal_big(value_1, value_2)) {
    val_1 = value_1;
    val_2 = value_2;
    // постановка знака
    if (sign_1 == 1) sign_res = 1;  // s21_set_bit_1_big(&tmp, 223);
  } else {
    val_1 = value_2;
    val_2 = value_1;
    // постановка знака
    // s21_set_bit_1_big(&tmp, 223);
    if ((sign_2 == 0 && sign_1 == 0)) sign_res = 1;
    // s21_set_bit_0_big(&tmp, 223);
    if ((sign_2 == 1 && sign_1 == 1)) sign_res = 0;
  }

  // (void)val_1;
  // (void)val_2;

  while (index_bit != 191) {
    bit_of_num_1 = s21_get_bit_big(&val_1, index_bit);
    bit_of_num_2 = s21_get_bit_big(&val_2, index_bit);

    if (!bit_of_num_1 && !bit_of_num_2) {  // оба бита = 0
      s21_set_bit_0_big(&tmp, index_bit);

    } else if (bit_of_num_1 && !bit_of_num_2) {  // бит первого = 1, второго = 0
      s21_set_bit_1_big(&tmp, index_bit);

    } else if (!bit_of_num_1 && bit_of_num_2) {  // бит первого = 0, второго = 0
      i = 1;  // с ее помощью смотрим следующии биты
      s21_set_bit_0_big(&tmp, index_bit);
      // цикл будет искать ближайший бит = 1
      while (!flag_bit_min) {
        if (s21_get_bit_big(&val_1, index_bit + i)) {
          s21_set_bit_0_big(&val_1, index_bit + i);
          flag_bit_min = 1;
        } else {
          s21_set_bit_1_big(&val_1, (index_bit + i));
          i++;
        }
      }
      s21_set_bit_1_big(&tmp, index_bit);
      flag_bit_min = 0;

    } else if (bit_of_num_1 && bit_of_num_2) {  // оба бита = 1
      s21_set_bit_0_big(&tmp, index_bit);
    }
    index_bit++;
  }

  if (func == 1) {
    s21_print_big_decimal_number(&tmp);
    int rewrite = check_big_decimal(tmp);
    if (rewrite == 3) {
      *result = tmp;
    } else if (power_of_result) {
      while (power_of_result && rewrite != 3) {
        s21_print_big_decimal_number(&tmp);
        s21_div_big(tmp, big_10, &tmp);
        s21_print_big_decimal_number(&tmp);
        // s21_round_big(tmp, &tmp);
        s21_truncate_big(tmp, &value_unsigned_truncated);
        s21_sub_big(tmp, value_unsigned_truncated, &fractional, 1);
        s21_print_big_decimal_number(&tmp);
        tmp = s21_round_banking_big(value_unsigned_truncated, fractional);
        // надо предусмотреть отрицательную степень
        s21_print_big_decimal_number(&tmp);

        /*
          При делении 0.00000000000000000002 / 2000000000 =
          1999999999.99999999999999999998
          оригинальный ответ уходит за 95 бит
          Если использовать раунд то он выдает округление до
          2000000000.0000000000000000000
          Если использовать div_10 то он выдает округление до
          s21_div_decimal_by_10_big(&tmp);
          1999999999.9999999999999999999
        */
        // s21_print_big_decimal_number(&tmp);
        power_of_result--;
        if (power_of_1 || power_of_2) {
          power_of_result -= s21_truncate_zero_big(&tmp);
          while (power_of_result < 0) {
            s21_mul_decimal_by_10_big(&tmp);
            power_of_result++;
          }
        }
        rewrite = check_big_decimal(tmp);
      }
      if (!power_of_result && rewrite != 3) {
        output = CONVERSATION_BIG;
      } else {
        output = CONVERSATION_OK;
        *result = tmp;
      }
    } else {
      output = CONVERSATION_BIG;
    }

    if (output == CONVERSATION_OK)
      s21_set_power_of_big_decimal(result, power_of_result);
    if (sign_res) s21_set_bit_1_big(result, 223);
    if (!sign_res) s21_set_bit_0_big(result, 223);

  } else {
    *result = tmp;
  }

  // s21_set_power_of_big_decimal(&tmp, power_of_result);
  // *result = tmp;
  // output = CONVERSATION_OK;
  // }

  return output;
}