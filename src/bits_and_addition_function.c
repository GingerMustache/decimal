#include "s21_decimal.h"

int s21_get_row(int bit) { return bit / 32; }
int s21_get_col(int bit) { return bit % 32; }

// Постановка знака
void s21_set_sign_of_int_and_float_number(s21_decimal *dec_num, float num_1,
                                          int case_of_type) {
  long int i = two_32 / 2;
  int float_in_32bit = *(int *)&num_1;
  if (case_of_type == 2) {
    if (float_in_32bit & i) {
      s21_set_bit_1(dec_num, 31);
    }
  } else {
    if (float_in_32bit & i) {
      s21_set_bit_1(dec_num, 127);
    }
  }
}

// Убираем степень
void s21_cut_exp(s21_decimal *a, int exp) {
  for (int i = exp; i > 0; i--) {
    s21_div_decimal_by_10(a);
  }
  s21_set_power_of_decimal(a, 0);
}

// Получить биты
int s21_get_bits(unsigned int bits, unsigned int num) {
  unsigned int mask = 1 << num;
  return (bits & mask) ? 1 : 0;
}

// Получить бит
int s21_get_bit(s21_decimal *dec_num, int index) {
  int getRow = s21_get_row(index);
  int getCol = s21_get_col(index);
  return (dec_num->bits[getRow] & (1 << getCol)) != 0;
}

// Поставить бит в 1
void s21_set_bit_1(s21_decimal *dec_num, int index) {
  int getRow = s21_get_row(index);
  int getCol = s21_get_col(index);
  dec_num->bits[getRow] |= (1 << getCol);
}

// Поставить бит в 0
void s21_set_bit_0(s21_decimal *dec_num, int index) {
  int getRow = s21_get_row(index);
  int getCol = s21_get_col(index);
  dec_num->bits[getRow] &= ~(1 << getCol);
}

void s21_set_dec_number_to_0(s21_decimal *src_num) {
  memset(&(src_num->bits), 0, sizeof(src_num->bits));
}

// Поставить биг децимал в 0
void s21_set_big_dec_number_to_0(s21_big_decimal *src_num) {
  memset(&(src_num->bits), 0, sizeof(src_num->bits));
}

// Децимал 0?
int s21_is_decimal_0(s21_decimal dec_num) {
  s21_decimal zero_decimal = {0};
  return s21_is_equal(zero_decimal, dec_num);
}

// Деление на 10, работает только для децимал, не биг
void s21_div_decimal_by_10(s21_decimal *num) {
  unsigned long long rem = 0, tmp;
  for (int i = 2; i >= 0; i--) {
    tmp = 0;
    tmp = (unsigned long long)num->bits[i] + rem;
    num->bits[i] = (unsigned int)(tmp / 10);
    rem = (tmp % 10) << 32;
  }
}

// Получить степень
int s21_get_power_of_decimal(s21_decimal src) {
  int index = 112;
  int result = 0;
  int scale = 0;
  while (index != 120) {
    if (s21_get_bit(&src, index)) {
      result += pow(2, scale);
    }
    index++;
    scale++;
  }
  if (result > 28) result = 0;
  return result;
}

// Получить децимал равный 1
void s21_set_dec_number_to_1(s21_decimal *src_num) {
  s21_decimal decimal_1 = {{1, 0, 0, 0}};
  *src_num = decimal_1;
}

// Постановка степени децимал
void s21_set_power_of_decimal(s21_decimal *src, int power) {
  if (power < 29) {
    for (int i = 112; i < 120; i++) {  // обнуление
      s21_set_bit_0(src, i);
    }
    s21_set_bits_from_int_to_decimal(power, src, 112);  // заполнение
  }
}

// для банковского округления --
// формирует  decimal = 0.5
s21_decimal s21_decimal_get_zerofive(void) {
  s21_decimal result = {{0x5, 0x0, 0x0, 0x10000}};

  return result;
}

// Проверка на четность
int s21_decimal_even(s21_decimal value) { return (value.bits[0] & 1) != 1; }

// Перевод, не помню зачем
void s21_from_unsigned_long_int_to_decimal(unsigned long int src,
                                           s21_decimal *dst) {
  int exp = 0;
  src = fabs((double)src);
  while (src >= pow(2, exp)) exp++;
  while (src) {
    if (src - pow(2, exp) < 0) {
      s21_set_bit_0(dst, exp);
    } else {
      src -= pow(2, exp);
      s21_set_bit_1(dst, exp);
    }
    exp--;
  }
}

// Степень float
int get_float_exp_from_string(char *str, int *sign_of_float_power) {
  int result = 0;
  char *ptr = str;
  while (*ptr) {
    if (*ptr == 'E') {
      if (*(ptr + 1) == '+') {
        *sign_of_float_power = 1;
      } else if (*(ptr + 1) == '-') {
        *sign_of_float_power = 2;
      }
      ++ptr;
      result = strtol(ptr, NULL, 10);
      break;
    }
    ++ptr;
  }

  return result;
}
// Обработка знака для сложения и вычитания
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

// Функция для тестов
int last_bits(int pow,  // первый аргумент заполняет степень, второй аргумент
                        // знак (0 или 1) для последнего бита массива
              int sign) {  // Возвращает число для последнего бита массива
  return (pow << 16) ^ (sign << 31);
}
