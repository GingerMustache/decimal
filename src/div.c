#include "s21_decimal.h"

/**
 * @brief
 * Основной функционал и алгоритм вычисления происходит в s21_div_big
 * В s21_div обработка входных ошибок, постановка знака и вывод результата
 * Да, это будет ужасно, да, но я все обьясню..
 */
// Деление
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
  } else {  // остальные случаи
    s21_set_dec_number_to_0(result);
    // берем знаки
    int sign_1 = s21_get_bit(&value_1, 127);
    int sign_2 = s21_get_bit(&value_2, 127);
    // Создание тонны переменных и переписывание в big_decimal
    int result_of_big_decimal = 0;
    s21_big_decimal big_value_1 = {0};
    s21_big_decimal big_value_2 = {0};
    s21_big_decimal big_result = {0};
    rewrite_decimal_to_big(&big_value_1, value_1);
    rewrite_decimal_to_big(&big_value_2, value_2);
    // устанавливаем знаки в (+)
    s21_set_bit_0(&value_2, 127);
    s21_set_bit_0(&value_1, 127);
    // начинаем вычисление
    result_of_big_decimal = s21_div_big(big_value_1, big_value_2, &big_result);
    if (!result_of_big_decimal) {
      rewrite_from_big_decimal_to_decimal(big_result, result);
    } else if (result_of_big_decimal == 1) {
      output = CONVERSATION_BIG;
    } else if (result_of_big_decimal == 2) {
      output = CONVERSATION_SMALL;
    }
    // постановка знака
    if (sign_1 != sign_2) {
      s21_set_bit_1(result, 127);
    } else {
      s21_set_bit_0(result, 127);
    }
  }

  return (output);
}

//------------------------Деление big_decimal----------------------//

int s21_div_big(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal *result) {
  int output = CONVERSATION_OK;

  s21_set_big_dec_number_to_0(result);
  // Создаем тонну переменных
  int rewrite = 3;
  s21_big_decimal tmp_result = {0};
  s21_big_decimal final_tmp_result = {0};
  s21_big_decimal reminder = {{1, 0, 0, 0, 0, 0, 0}};
  int power_of_value_2 = 0;
  int check_reminder = 0;
  int power_of_result = 0;
  int power_of_1 = s21_get_power_of_big_decimal(value_1);
  int power_of_2 = s21_get_power_of_big_decimal(value_2);
  // Ставим степень в 0
  s21_set_power_of_big_decimal(&value_1, 0);
  s21_set_power_of_big_decimal(&value_2, 0);
  // еще немного переменных
  s21_big_decimal tmp_1 = value_1;
  s21_big_decimal tmp_2 = value_2;
  s21_big_decimal prev_value = {0};
  s21_big_decimal _2 = {{2, 0, 0, 0, 0, 0, 0}};

  power_of_result += power_of_1 - power_of_2;  // Установка степени
  /*
    1.  100100 / 10

    Если 10 меньше то двигаем влево до тех пор, пока не станет больше или равен
    Если больше, то если не было перехода в верхний инт (переход из 0 инта в
    структуре к 1 и тд), то делаем сдвиг вправо, если был переход, то возвращаем
    значени сохраненное в prev_value

    2.  100100
            10 <--
    3.  100100
      1000000

    4.  Вычитаем и смотрим остаток, если есть остаток то деление продолжается
  */
  while (!s21_is_decimal_0_big(reminder) && rewrite == 3) {
    s21_set_big_dec_number_to_0(&reminder);
    while (!check_reminder) {
      /*
        Проверка перехода в новый инт
        Если бит переходит в инт выше, то сдвиг влево меняется
        на умножение на два
      */
      for (; s21_is_less_big(tmp_2, tmp_1); power_of_value_2++) {
        if ((s21_get_bit_big(&tmp_2, 31)) || (s21_get_bit_big(&tmp_2, 63)) ||
            (s21_get_bit_big(&tmp_2, 95)) || (s21_get_bit_big(&tmp_2, 127)) ||
            (s21_get_bit_big(&tmp_2, 159))) {
          prev_value = tmp_2;
          s21_big_mul(tmp_2, _2, &tmp_2, 0);  // умножение на 2
        } else {
          prev_value = tmp_2;
          shift_big_bit_left(&tmp_2, 1, 0, 5);  // сдвиг влево
        }
      }
      if (s21_is_greater_big(tmp_2, tmp_1)) {  // делитель больше делимого?
        if (power_of_value_2 > 27) {
          tmp_2 = prev_value;  // возврат
        } else {
          shift_bit_right_big(&tmp_2, 1, 0);  // сдвиг вправо
        }
        power_of_value_2 -= 1;  // был сдвиг, убавляем степень сдвига
      }
      if (power_of_value_2 < 0) {  // меньше 0 -> обратно
        tmp_2 = value_2;
      } else {
        /*
          Делитель меньше или равен делителю
          Вычитаем
        */
        if (s21_is_less_or_equal_big(tmp_2, tmp_1)) {
          s21_sub_big(tmp_1, tmp_2, &tmp_1, 0);
          s21_set_bit_1_big(&tmp_result, power_of_value_2);
          tmp_2 = value_2;
        }
      }
      // обнуляемся и чекаем остаток
      power_of_value_2 = 0;
      reminder = tmp_1;
      check_reminder = s21_is_less_big(reminder, value_2);
    }
    // записываем в результат целую часть от деления
    s21_big_add(final_tmp_result, tmp_result, &final_tmp_result, 0);
    // Остаток есть?
    if (!s21_is_decimal_0_big(reminder)) {
      /*
        Если остаток есть то домножаем его и конечный результат на 10, пока
        остаток не станет равен или больше чем делитель
        10/3 = 3 и остаток 1 -> 1/3 неа, умножаем на 10 остаток и конечный
        результат, 10/3 ага...
      */
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
  // проверка степени
  while (power_of_result < 0) {
    s21_mul_decimal_by_10_big(&final_tmp_result);
    power_of_result++;
  }
  // помещается в decimal?
  rewrite = check_big_decimal(final_tmp_result);
  if (rewrite != 3 && power_of_result == 0) {
    return (output = CONVERSATION_BIG);
  }
  // постановка степени
  if (output == CONVERSATION_OK && power_of_result <= 28) {
    s21_set_power_of_big_decimal(&final_tmp_result, power_of_result);
    *result = final_tmp_result;
  } else {
    output = CONVERSATION_SMALL;
  }

  return (output);
}
