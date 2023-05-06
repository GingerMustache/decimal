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

//------------------------output----------------------//
#define CONVERSATION_ERROR 1  // надо проверить все функции!

#define CONVERSATION_OK 0
#define CONVERSATION_BIG 1
#define CONVERSATION_SMALL 2
#define CONVERSATION_DIV_ZERO 3

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

typedef struct {
  unsigned int bits[7]; /*
                [0 - 2] - обычный децимал (0-95)
                [3 - 5] - доп децимал (96 - 191)
                [6]
                192 - 207 - not used
                208 - 215 - scale
                216 - 222 - not used
                223 - sign
                 */
} s21_big_decimal;

//------------------------Конверторы----------------------//
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_float_to_decimal(float val, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

//------------------------Арифметика----------------------//
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

//------------------------Другие функции----------------------//
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
s21_decimal s21_round_banking(s21_decimal integral, s21_decimal fractional);

//------------------------Сравнение----------------------//
int s21_is_greater(s21_decimal num_1, s21_decimal num_2);
int s21_is_greater_or_equal(s21_decimal num_1, s21_decimal num_2);
int s21_is_less(s21_decimal num_1, s21_decimal num_2);
int s21_is_less_or_equal(s21_decimal num_1, s21_decimal num_2);
int s21_is_equal(s21_decimal num_1, s21_decimal num_2);
int s21_is_not_equal(s21_decimal num_1, s21_decimal num_2);

//------------------------Вспомогательные функции----------------------//
void s21_set_sign_of_int_and_float_number(s21_decimal *dec_num, float num_1,
                                          int case_of_type);
void s21_set_dec_number_to_0(s21_decimal *src_num);
void s21_set_dec_number_to_1(s21_decimal *src_num);
void s21_print_decimal_number(s21_decimal *num);
int s21_get_float_bit(int num, int index);
int s21_rewrite_float_bits_to_buff(s21_decimal *buff, float val);
void s21_set_bits_from_int_to_decimal(int src, s21_decimal *dst,
                                      int which_int_part_to_fill);
void s21_from_unsigned_long_int_to_decimal(unsigned long int src,
                                           s21_decimal *dst);
int get_float_exp_from_string(char *str, int *sign_of_float_power);
int s21_get_power_of_decimal(s21_decimal src);
void s21_div_decimal_by_10(s21_decimal *num);
int s21_mul_decimal_by_10(s21_decimal *num);
s21_decimal s21_decimal_get_inf(void);
void s21_set_power_of_decimal(s21_decimal *src, int power);
int s21_sign_handle(s21_decimal *value_1, s21_decimal *value_2,
                    s21_decimal *result, int function);
int s21_normalize(s21_decimal *num_1, s21_decimal *num_2);
void s21_truncate_zero(s21_decimal *value, int count_zero);
int s21_is_decimal_0(s21_decimal dec_num);
void s21_cut_exp(s21_decimal *a, int exp);
int s21_get_bits(unsigned int bits, unsigned int num);

s21_decimal s21_decimal_get_zerofive(void);
int s21_decimal_even(s21_decimal value);
s21_decimal s21_round_banking(s21_decimal integral, s21_decimal fractional);
int s21_is_max_decimal(s21_decimal num);

//------------------------Сдвиги(вспомогательные)----------------------//
int s21_shift_bits(s21_decimal *dec_num, int index);
int s21_shift_31(s21_decimal *dec_num, int flg_31, int flg_63);
int s21_shift_63(s21_decimal *dec_num);
int check_index_shift(s21_decimal dec_num, int index);
void shift_bit_right(s21_decimal *value, int count);
void shift_bit_left(s21_decimal *value, int count);

//------------------------Работа с битами----------------------//
int s21_get_col(int bit);
int s21_get_row(int bit);
int s21_get_bit(s21_decimal *dec_num, int index);
void s21_set_bit_1(s21_decimal *dec_num, int index);
void s21_set_bit_0(s21_decimal *dec_num, int index);
int s21_inverse_bit(s21_decimal *dec_num, int index);
int s21_get_bit_from_not_decimal(float src, int index);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

//------------------------Работа с big_decimal----------------------//

void s21_print_big_decimal_number(s21_big_decimal *num);
void s21_set_bit_0_big(s21_big_decimal *dec_num, int index);
void s21_set_bit_1_big(s21_big_decimal *dec_num, int index);
int s21_get_bit_big(s21_big_decimal *dec_num, int index);

int s21_big_add(s21_big_decimal big_value_1, s21_big_decimal big_value_2,
                s21_big_decimal *big_result, int func);
int s21_big_mul(s21_big_decimal big_value_1, s21_big_decimal big_value_2,
                s21_big_decimal *result, int func);
int s21_sub_big(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal *result, int func);
int s21_div_big(s21_big_decimal value_1, s21_big_decimal value_2,
                s21_big_decimal *result);

void rewrite_decimal_to_big(s21_big_decimal *big_decimal, s21_decimal decimal);
void rewrite_from_big_decimal_to_decimal(s21_big_decimal big_decimal,
                                         s21_decimal *decimal);
int s21_get_power_of_big_decimal(s21_big_decimal src);
void s21_set_bits_from_int_to_big_decimal(int src, s21_big_decimal *dst,
                                          int which_int_part_to_fill);
void s21_set_power_of_big_decimal(s21_big_decimal *src, int power);
void s21_set_big_dec_number_to_0(s21_big_decimal *src_num);
int s21_truncate_zero_big(s21_big_decimal *value);

int s21_shift_bits_big(s21_big_decimal *dec_num, int index);
int s21_shift_31_big(s21_big_decimal *dec_num, int *flg_63, int *flg_95,
                     int *flg_127, int *flg_159, int *flg_overlow);
int s21_shift_63_big(s21_big_decimal *dec_num, int *flg_95, int *flg_127,
                     int *flg_159, int *flg_overlow);
int s21_shift_95_big(s21_big_decimal *dec_num, int *flg_127, int *flg_159,
                     int *flg_overlow);
int s21_shift_127_big(s21_big_decimal *dec_num, int *flg_159, int *flg_overlow);
int s21_shift_159_big(s21_big_decimal *dec_num);
void shift_bit_right_big(s21_big_decimal *value, int count, int number_shift);
void shift_big_bit_left(s21_big_decimal *value, int count, int number_shift,
                        int end_shift);
void twist_bit_big(s21_big_decimal *dec_num, int first, int second);
int s21_normalize_big(s21_big_decimal *num_1, s21_big_decimal *num_2);
int s21_is_decimal_0_big(s21_big_decimal dec_num);
void s21_set_dec_number_to_1_big(s21_big_decimal *src_num);

int s21_is_equal_big(s21_big_decimal num_1, s21_big_decimal num_2);
int s21_is_greater_big(s21_big_decimal num_1, s21_big_decimal num_2);
int s21_is_less_big(s21_big_decimal num_1, s21_big_decimal num_2);
int s21_is_less_or_equal_big(s21_big_decimal num_1, s21_big_decimal num_2);
int s21_is_greater_or_equal_big(s21_big_decimal num_1, s21_big_decimal num_2);

int s21_truncate_big(s21_big_decimal value, s21_big_decimal *result);
s21_big_decimal s21_round_banking_big(s21_big_decimal integral,
                                      s21_big_decimal fractional);

s21_big_decimal s21_decimal_get_zerofive_big(void);
int s21_decimal_even_big(s21_big_decimal value);
int s21_round_big(s21_big_decimal value, s21_big_decimal *result);

int s21_mul_decimal_by_10_big(s21_big_decimal *num);
int s21_div_decimal_by_10_big(s21_big_decimal *value_1,
                              s21_big_decimal *out_reminder);

int check_big_decimal(s21_big_decimal big_tmp);

#endif  // _SRC_S21_DECIMAL_H_