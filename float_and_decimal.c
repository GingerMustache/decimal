#include "s21_decimal.h"

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

int s21_rewrite_float_bits_to_buff(s21_decimal *buff,
                                   float val) {  //  зачем написал ?? хз..
  int float_in_32bit = *(int *)&val;
  int index = 31;
  long int i = two_32 / 2;  // 2^32
  int scale = 0;

  while (i != 1) {
    if (float_in_32bit & i && i == two_32 / 2) {  // проверка знака числа
      // set_sign_of_number(buff, (int)val);
      s21_set_bit_1(buff, 31);
      // мжно заменить просто на постановку бита в 31 бит
    } else if ((float_in_32bit & i) &&
               (index < 31 && index > 22)) {  // учет степени
      scale += pow(2, index - 23);
      s21_set_bit_1(buff, index);
      // если index от 30 до 23 то это степень, относительно 127
    } else if (float_in_32bit & i) {  // постановка бита в 1
      s21_set_bit_1(buff, index);
    } else {  // постановка бита в 0
      s21_set_bit_0(buff, index);
    }
    i >>= 1;
    index--;
  }
  return (scale -= 127);
}

int s21_get_float_exp_from_string(char *str) {
  int result = 0;
  char *ptr = str;
  while (*ptr) {
    if (*ptr == 'E') {
      ++ptr;
      result = strtol(ptr, NULL, 10);
      break;
    }
    ++ptr;
  }

  return result;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int output = CONVERSATION_OK;
  int count_significant_decimal_digits = 6;
  int zero_scale = 0;
  int scale = 0;
  int scale_of_float = 0;
  unsigned long int result = 0;
  char buffer_flt[64];

  if (!dst) {
    output = CONVERSATION_ERROR;
  } else if (src == 0.0) {
    output = CONVERSATION_OK;
    s21_set_dec_number_to_0(dst);
    s21_set_sign_of_int_and_float_number(dst, src, CASE_OF_DECIMAL);
  } else if (isinf(src) || isnan(src)) {
    output = CONVERSATION_ERROR;
  } else if (fabsf(src) > MAX_FLOAT_TO_CONVERT) {
    output = CONVERSATION_ERROR;
  } else {
    s21_set_dec_number_to_0(dst);  // обнуление dst
    printf("Исходное число: %.6E\n", src);
    s21_set_sign_of_int_and_float_number(dst, src, CASE_OF_DECIMAL);
    src = fabsf(src);

    while (src < 1 && zero_scale < 29) {
      src *= 10;
      zero_scale++;
    }

    if (zero_scale <= 28) {
      if (zero_scale >= 22) {
        count_significant_decimal_digits = 28 - zero_scale;
      }
      sprintf(buffer_flt, "%.6E", fabs(src));
      scale_of_float = s21_get_float_exp_from_string(buffer_flt) + 1;
      if (scale_of_float > 28) {
        output = CONVERSATION_ERROR;
      }

      for (int i = 0;
           buffer_flt[i] != '\0' && i <= count_significant_decimal_digits + 1;
           i++) {
        if (buffer_flt[i] != '.') {
          result = result * 10 + (buffer_flt[i] - '0');
          scale++;
        }
      }

      sprintf(buffer_flt, "%ld", result);
      int lenght_of_buffer = strlen(buffer_flt);

      while (buffer_flt[lenght_of_buffer - 1] == '0') {
        result /= 10;
        scale--;
        lenght_of_buffer--;
      }
      printf("Строка без точки: %ld\n", result);
      if (zero_scale == 0) {
        scale = scale - scale_of_float;
      } else if (zero_scale == 28) {
        scale = zero_scale;
      } else {
        scale = zero_scale + scale - scale_of_float;
      }
      while (scale < 0) {
        if (zero_scale) {
          result *= 10;
          scale++;
        } else {
          scale++;
        }
      }
    } else {
      output = CONVERSATION_ERROR;
    }
  }
  printf("Строка идущая в decimal: %ld\n", result);
  if (scale <= 28 && output == CONVERSATION_OK) {
    s21_from_unsigned_long_int_to_decimal(result, dst);
    s21_set_bits_from_int_to_decimal(scale, dst, 112);
    printf("децимал после обработкой\n");
    s21_print_decimal_number(dst);
    printf("scale = %d\n", scale);
    return (output);
  } else {
    s21_set_dec_number_to_0(dst);
    return (output);
  }

  // есть проблема с воводом ..,1 0.1.. 0.01, только когда комп тупит
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int output = CONVERSATION_ERROR;
  if (dst) {
    *dst = 0.0;
    int index = 0;
    double power = s21_get_power_of_decimal(src);
    if (power < 29) {
      double tmp = 0.0;
      int get_sign = s21_get_bit(&src, 127);

      while (index != 96) {
        if (s21_get_bit(&src, index)) {
          tmp += pow(2, index);
        }
        index++;
      }
      tmp /= pow(10, power);
      if (get_sign) tmp *= -1;

      *dst = (float)tmp;

      output = CONVERSATION_OK;
    }
  }
  return output;
}