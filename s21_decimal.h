#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef _SRC_S21_DECIMAL_H_
#define _SRC_S21_DECIMAL_H_

#define two_32 4294967296
#define MAX_INT 2147483647
#define MIN_INT -2147483648
#define MAX_FLOAT_TO_CONVERT 79228157791897854723898736640.0f
#define MIN_FLOAT_TO_CONVERT \
  0.00000000000000000000000000010000000031710768509710513471352647538147514756461109f

#define CONVERSATION_ERROR 1
#define CONVERSATION_OK 0

#define CASE_OF_INT 1
#define CASE_OF_FLOAT 2
#define CASE_OF_DECIMAL 3

typedef struct {
  unsigned int bits[4]; /*
                 [3]
                 96 - 111 - not used
                 112 - 119 - scale 10^..
                 120 - 126 - not used
                 127 - sign
                 */
} s21_decimal;

int s21_get_col(int bit);
int s21_get_row(int bit);
void s21_set_sign_of_int_and_float_number(s21_decimal *dec_num, float num_1,
                                          int case_of_type);
int s21_get_bit(s21_decimal *dec_num, int index);
void s21_set_bit_1(s21_decimal *dec_num, int index);
void s21_set_bit_0(s21_decimal *dec_num, int index);
void s21_set_dec_number_to_0(s21_decimal *src_num);
int s21_inverse_bit(s21_decimal *dec_num, int index);
void s21_shift_bits(s21_decimal *dec_num, int index);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
void s21_print_decimal_number(s21_decimal *num);
int s21_is_greater(s21_decimal num_1, s21_decimal num_2);
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_get_float_bit(int num, int index);
int s21_from_float_to_decimal(float val, s21_decimal *dst);
int s21_rewrite_float_bits_to_buff(s21_decimal *buff, float val);
void s21_set_bits_from_int_to_decimal(int src, s21_decimal *dst,
                                      int which_int_part_to_fill);
void s21_from_unsigned_long_int_to_decimal(unsigned long int src,
                                           s21_decimal *dst);
int s21_get_bit_from_not_decimal(float src, int index);
int s21_get_float_exp_from_string(char *str);
int s21_get_power_of_decimal(s21_decimal src);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
void s21_div_decimal_by_10(s21_decimal *num);
s21_decimal s21_decimal_get_inf(void);
void s21_set_power_of_decimal(s21_decimal *src, int power);
int s21_truncate(s21_decimal value, s21_decimal *result);

#endif  // _SRC_S21_DECIMAL_H_