#include "s21_decimal.h"

// Конвертер float->decimal
int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int output = CONVERSATION_OK;
  int count_significant_decimal_digits = 6;
  int zero_scale = 0;
  int scale = 0;
  int scale_of_float = 0;
  int sign_of_float_power = 0;
  unsigned long int result = 0;
  char buffer_flt[64];

  if (src == 0.0) {
    output = CONVERSATION_OK;
    s21_set_dec_number_to_0(dst);
    s21_set_sign_of_int_and_float_number(dst, src, CASE_OF_DECIMAL);
  } else if (isinf(src) || isnan(src)) {
    output = CONVERSATION_ERROR;
  } else if (fabsf(src) > MAX_FLOAT_TO_CONVERT) {
    output = CONVERSATION_ERROR;
  } else {
    s21_set_dec_number_to_0(dst);  // обнуление dst
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
      scale_of_float =
          get_float_exp_from_string(buffer_flt, &sign_of_float_power) + 1;
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

      if (zero_scale == 0) {
        if (sign_of_float_power == 1 && scale_of_float > 6) {
          scale = 0;
        } else {
          scale = scale - scale_of_float;
        }
      } else if (zero_scale == 28) {
        scale = zero_scale;
      } else {
        scale = zero_scale + scale - scale_of_float;
      }
      while (scale < 0) {
        result *= 10;
        scale++;
      }
    } else {
      output = CONVERSATION_ERROR;
    }
  }
  if (scale <= 28 && output == CONVERSATION_OK) {
    s21_from_unsigned_long_int_to_decimal(result, dst);
    s21_set_bits_from_int_to_decimal(scale, dst, 112);
    return (output);
  } else {
    s21_set_dec_number_to_0(dst);
    return (output);
  }
}

// Конвертер decimal->float
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
