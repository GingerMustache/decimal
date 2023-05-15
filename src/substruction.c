#include "s21_decimal.h"

// Вычитание
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int output = CONVERSATION_ERROR;
  int sign_1 = s21_get_bit(&value_1, 127);
  int sign_2 = s21_get_bit(&value_2, 127);
  int sign_res = 0;
  output = s21_sign_handle(&value_1, &value_2, result, 1);

  if (output == 2) {
    s21_decimal val_1 = {0};
    s21_decimal val_2 = {0};
    s21_decimal tmp = {0};
    s21_set_dec_number_to_0(result);

    int big_decimal_output = 0;

    s21_big_decimal big_value_1 = {0};
    s21_big_decimal big_value_2 = {0};
    s21_big_decimal big_result = {0};

    rewrite_decimal_to_big(&big_value_1, value_1);
    rewrite_decimal_to_big(&big_value_2, value_2);

    // обработка знака и чисел при вычитании
    if (s21_is_greater_or_equal_big(big_value_1, big_value_2)) {
      val_1 = value_1;
      val_2 = value_2;
      if (sign_1 == 1) sign_res = 1;
    } else {
      val_1 = value_2;
      val_2 = value_1;
      rewrite_decimal_to_big(&big_value_1, val_1);
      rewrite_decimal_to_big(&big_value_2, val_2);
      if ((sign_2 == 0 && sign_1 == 0)) sign_res = 1;
      if ((sign_2 == 1 && sign_1 == 1)) sign_res = 0;
    }
    // переписывание в big_decimal и обработка результата
    big_decimal_output = s21_sub_big(big_value_1, big_value_2, &big_result, 1);
    if (!big_decimal_output) {
      rewrite_from_big_decimal_to_decimal(big_result, &tmp);
      *result = tmp;
      if (sign_res) s21_set_bit_1(result, 127);
      if (!sign_res) s21_set_bit_0(result, 127);
      output = CONVERSATION_OK;
    } else if (big_decimal_output == 1) {
      output = CONVERSATION_BIG;
    } else if (big_decimal_output == 2) {
      output = CONVERSATION_SMALL;
    }
  }
  return output;
}

//------------------------Вычитание big_decimal----------------------//
// func - переменная показывающая необходимо ли использовать функционал
// переполнения
int s21_sub_big(s21_big_decimal val_1, s21_big_decimal val_2,
                s21_big_decimal *result, int func) {
  int output = CONVERSATION_OK;

  s21_set_bit_0_big(&val_1, 223);
  s21_set_bit_0_big(&val_2, 223);
  s21_big_decimal tmp = {0};

  s21_big_decimal fractional = {0};
  s21_big_decimal value_unsigned_truncated = {0};

  int index_bit = 0;
  int i = 1;
  int flag_bit_min = 0;
  int bit_of_num_1 = 0;
  int bit_of_num_2 = 0;
  int power_of_result = 0;
  int power_of_1 = s21_get_power_of_big_decimal(val_1);
  int power_of_2 = s21_get_power_of_big_decimal(val_2);

  if ((power_of_1 && power_of_2) || (power_of_1 || power_of_2)) {
    power_of_result = s21_normalize_big(&val_1, &val_2);  // нормализация
  }

  while (index_bit != 191) {
    bit_of_num_1 = s21_get_bit_big(&val_1, index_bit);
    bit_of_num_2 = s21_get_bit_big(&val_2, index_bit);
    // оба бита = 0
    if (!bit_of_num_1 && !bit_of_num_2) {
      s21_set_bit_0_big(&tmp, index_bit);
      // бит первого = 1, второго = 0
    } else if (bit_of_num_1 && !bit_of_num_2) {
      s21_set_bit_1_big(&tmp, index_bit);
      // бит первого = 0, второго = 0
    } else if (!bit_of_num_1 && bit_of_num_2) {
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
  // обработка переполнений и округлений
  if (func == 1) {
    int rewrite = check_big_decimal(tmp);
    if (rewrite == 3) {
      *result = tmp;
    } else if (power_of_result) {
      while (power_of_result && rewrite != 3) {
        s21_set_power_of_big_decimal(&tmp, 1);
        s21_truncate_big(tmp, &value_unsigned_truncated);
        s21_sub_big(tmp, value_unsigned_truncated, &fractional, 1);
        tmp = s21_round_banking_big(value_unsigned_truncated, fractional);
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

  } else {
    *result = tmp;
  }

  return output;
}
