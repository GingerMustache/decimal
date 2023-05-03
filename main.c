#include "s21_decimal.h"

int s21_normalize(s21_decimal *num_1, s21_decimal *num_2) {
  int power_num_1 = s21_get_power_of_decimal(*num_1);
  int power_num_2 = s21_get_power_of_decimal(*num_2);
  s21_set_power_of_decimal(num_1, 0);  // ставим степени чисел в 0
  s21_set_power_of_decimal(num_2, 0);
  s21_decimal bit_number_10 = {10, 0, 0, 0};
  // printf("\n-decimal_numbers from normalize-\n");
  // printf("bit_num_10\n");
  // s21_print_decimal_number(&bit_number_10);
  // printf("1st\n");
  // s21_print_decimal_number(num_1);
  // printf("2nd\n");
  // s21_print_decimal_number(num_2);

  if (power_num_1 < power_num_2) {
    while (power_num_2 - power_num_1) {
      s21_mul(*num_1, bit_number_10, num_1);
      power_num_1++;
    }
    return (power_num_1);
  } else if (power_num_2 < power_num_1) {
    while (power_num_1 - power_num_2) {
      s21_mul(*num_2, bit_number_10, num_2);
      power_num_2++;
    }
    return (power_num_2);
  }
  return (power_num_1);
}

void s21_print_decimal_number(s21_decimal *num) {
  int count = 31;
  long int i = two_32 / 2;   // 2^32
  short printable_part = 3;  // 0 = 1 strong to print, 1 = 2 ...

  while (printable_part != -1) {
    if (count < 23 && count > 14) {
      printf(" ");
    }
    if (num->bits[printable_part] & i && (i == two_32 / 2) &&
        printable_part == 3) {
      printf("- ");
    } else if (!(num->bits[printable_part] & i) && (i == two_32 / 2) &&
               printable_part == 3) {
      printf("+ ");
    } else if (num->bits[printable_part] & i) {
      printf("%d", 1);
    } else if (!(num->bits[printable_part] & i) &&
               !(count < 31 && count > 23) && !(count < 16 && count >= 0)) {
      printf("%d", 0);
    }
    if (i == 1) {
      printf("\n");
      printable_part--;
      i = two_32;
    }
    i >>= 1;
    count--;
  }
  printf("\n");
  // printf("number of printed bit = %d\n", count);
}

void s21_print_big_decimal_number(s21_big_decimal *num) {
  int flg_half_part = 1;
  int count = 31;
  long int i = two_32 / 2;   // 2^32
  short printable_part = 6;  // 0 = 1 strong to print, 1 = 2 ...

  while (printable_part != -1) {
    if (count < 23 && count > 14) {
      printf(" ");
    }
    if (printable_part == 2 && flg_half_part) {
      printf("--- ordinar decimal part ---\n");
      flg_half_part--;
    }
    if (num->bits[printable_part] & i && (i == two_32 / 2) &&
        printable_part == 6) {
      printf("- ");
    } else if (!(num->bits[printable_part] & i) && (i == two_32 / 2) &&
               printable_part == 6) {
      printf("+ ");
    } else if (num->bits[printable_part] & i) {
      printf("%d", 1);
    } else if (!(num->bits[printable_part] & i) &&
               !(count < 31 && count > 23) && !(count < 16 && count >= 0)) {
      printf("%d", 0);
    }
    if (i == 1) {
      printf("\n");
      printable_part--;
      i = two_32;
    }
    i >>= 1;
    count--;
  }
  printf("\n");
  // printf("number of printed bit = %d\n", count);
}

int main(void) {
  s21_decimal bit_number = {0};
  s21_decimal bit_number_2 = {0};
  s21_decimal bit_number_3 = {0};
  s21_decimal bit_number_result = {0};
  s21_decimal bit_number_result_2 = {0};
  int dst = 245;

  s21_big_decimal big_bit_number = {0};
  s21_big_decimal big_bit_number_2 = {0};
  s21_big_decimal big_bit_number_result = {0};
  s21_big_decimal big_bit_number_result_2 = {0};

  // ---------работа с int------------

  // s21_from_int_to_decimal(1, &bit_number);
  // s21_from_int_to_decimal(1, &bit_number_2);
  // s21_from_int_to_decimal(2, &bit_number_3);
  // dst = s21_sub(bit_number, bit_number_2, &bit_number_result);
  // dst = s21_add(bit_number, bit_number_2, &bit_number_result);

  // printf("first dec number\n");
  // s21_print_decimal_number(&bit_number);
  // printf("second dec number\n");
  // s21_print_decimal_number(&bit_number_2);
  // s21_sub(bit_number, bit_number_2, &bit_number_result);
  // printf("result dec number\n");
  // s21_print_decimal_number(&bit_number_result);

  // // set_dec_number_to_0(&bit_number);
  // print_decimal_number(&bit_number_result);
  // int res_2 = s21_from_decimal_to_int(bit_number_2, &dst_2);

  // (void)ress;
  // (void)ress_2;
  (void)big_bit_number_result_2;
  (void)big_bit_number_result;
  (void)big_bit_number;
  (void)big_bit_number_2;
  (void)bit_number;
  (void)bit_number_2;
  (void)bit_number_3;
  (void)bit_number_result;
  (void)bit_number_result_2;
  (void)dst;

  // printf("code_of _errors = %d\n", ress);
  // printf("code_of _errors_2 = %d\n", ress_2);
  // int isgreater = 0;
  // isgreater = s21_is_greater(bit_number, bit_number_2);
  // printf("\nfirst is greater(1,0)  -  %d", isgreater);
  // int res = 0;
  // printf("\n");
  // printf("\nres = %d", res);
  // printf("\n");
  // print_decimal_number(&bit_number);

  // --------работа с deciimal--------

  int i = 5;
  while (i != 226) {
    if (i <= 95) {
      s21_set_bit_1(&bit_number, i);  // число
    }
    // if (i <= 95) {
    //   s21_set_bit_1(&bit_number_2, i);  // число
    // }
    // // if (i == 127) {
    //   s21_set_bit_1(&bit_number, i);  // знак
    // }
    // if (i == 112) {
    //   s21_set_bit_1(&bit_number_2, i);  // степень
    // }
    i++;
  }

  // s21_set_power_of_decimal(&bit_number, 29);
  // s21_set_power_of_decimal(&bit_number_2, 28);

  // printf("Output = %d\n", s21_from_float_to_decimal(
  //                             0.0000000000000000000000000002, &bit_number));
  printf("Output = %d\n", s21_from_float_to_decimal(8.62345, &bit_number_2));
  // printf("децимал перед входом в функцию\n");

  printf("\nfirst dec number\n");
  s21_print_decimal_number(&bit_number);
  rewrite_decimal_to_big(&big_bit_number, bit_number);
  printf("\nbig_decimal_number first\n");
  s21_print_big_decimal_number(&big_bit_number);

  // printf("\nsecond dec number\n");
  // s21_print_decimal_number(&bit_number_2);
  // rewrite_decimal_to_big(&big_bit_number_2, bit_number_2);
  printf("big_decimal_number second\n");
  s21_print_big_decimal_number(&big_bit_number_2);

  // s21_truncate_big(big_bit_number, &big_bit_number_result);
  // s21_round_big(big_bit_number, &big_bit_number_result);

  // printf("\nbit_res\n");

  printf("res of func = %d\n",
         s21_add(bit_number, bit_number_2, &bit_number_result));
  //  s21_div_decimal_by_10_big(&big_bit_number));
  // s21_truncate_big(big_bit_number_result, &big_bit_number_result_2);

  // printf("big_decimal result\n");
  // s21_print_big_decimal_number(&big_bit_number_result);
  printf("decimal result\n");
  s21_print_decimal_number(&bit_number_result);
  // s21_round_big(big_bit_number_2, &big_bit_number_result_2);
  // printf("big_decimal result_2\n");
  // s21_print_big_decimal_number(&big_bit_number_result);

  // printf("result of function = %d\n",
  //        s21_div(bit_number, bit_number_2, &bit_number_result));
  // // normalize(&bit_number, &bit_number_2);
  // printf( res = %d\n", s21_round(bit_number, &bit_number_result));
  // printf("res of function_1 %d\n",
  //        s21_div(bit_number, bit_number_2, &bit_number_result));
  // printf("result dec number\n");
  // printf("right conv = %d\n", s21_mul_decimal_by_10(&bit_number_result));
  // s21_print_decimal_number(&bit_number_result);
  // printf("wrong conv = %d\n", s21_mul_decimal_by_10(&bit_number_result));

  // printf("res of function_2 %d\n",
  //        s21_mul(bit_number_result, bit_number_2, &bit_number_result));
  // s21_print_decimal_number(&bit_number_result);

  // printf("res of function_3 %d\n",
  //        s21_mul(bit_number_result, bit_number_2, &bit_number_result));
  // s21_print_decimal_number(&bit_number_result);

  // printf("res of function_4 %d\n",  // забивается весь децимал, но не
  // overflow
  //        s21_mul(bit_number_result, bit_number_3, &bit_number_result));
  // s21_print_decimal_number(&bit_number_result);

  // // printf("Output = %d\n", s21_from_float_to_decimal(1.1,
  // &bit_number_3));

  // printf("res of function_5 %d\n",
  //        s21_mul(bit_number_result, bit_number_3, &bit_number_result));
  // s21_print_decimal_number(&bit_number_result);
  // s21_set_power_of_decimal(&bit_number_result, 0);
  // printf("\n");
  // s21_print_decimal_number(&bit_number_result);

  // printf("i = %d\n", i);
  // printf("децимал перед входом в функцию\n");
  // // div_decimal_by_10(&bit_number);
  // set_power_of_decimal(&bit_number, 0);
  // print_decimal_number(&bit_number);
  // printf("децимал после входа в функцию\n");
  // set_power_of_decimal(&bit_number, 4);
  // print_decimal_number(&bit_number);
  // int res_int = s21_from_decimal_to_int(bit_number, &dst);
  // printf("int from decimal = %d\n", dst);
  // printf("result of func = %d\n", res_int);

  // int isgreater = 0;
  // isgreater = s21_is_greater(bit_number, bit_number_2);
  // printf("is greater - %d\n", isgreater);
  // // // int index = 0;
  // // // int res = 0;
  // // int dst = 0;
  // // // int res_int = s21_from_int_to_decimal(2147483647, &bit_number_2);
  // // // int res_int_2 = s21_from_int_to_decimal(2147483647, &bit_number);
  // // dst = s21_add(bit_number, bit_number_2, &bit_number_result);
  // // (void)dst;
  // (void)res_int;
  // // // (void)res_int_2;
  // print_decimal_number(&bit_number);
  // print_decimal_number(&bit_number_2);
  // print_decimal_number(&bit_number_result);

  // -----------работа с float-----------

  // float val = 1000;
  // float result = 0.0;
  // printf("%.6E", val);
  // printf("%d\n", rewrite_float_bits_to_buff(&bit_number, i));
  // printf("Output = %d\n", s21_from_float_to_decimal(val, &bit_number));
  // printf("децимал перед входом в функцию\n");
  // s21_print_decimal_number(&bit_number);
  // printf("децимал после входа в функцию\n");
  // printf("Output = %d\n", s21_negate(bit_number, &bit_number_result));
  // s21_print_decimal_number(&bit_number_result);
  // printf("Power = %d\n", get_power_of_decimal(bit_number));
  // printf("output = %d\n", s21_from_decimal_to_float(bit_number,
  // &result)); printf("val = %E\n", val); printf("result = %Ef\n", result);
  // (void)bit_number;
  // (void)res;
  return 0;
}

/*
  варианты для проверки
  -умножения
  1.printf("Output = %d\n", s21_from_float_to_decimal(0.234, &bit_number));
    printf("Output = %d\n", s21_from_float_to_decimal(-2.56745, &bit_number_2));
    // при этой проверки остается 0 в дробной части, но его убирать не надо -
      степень все решает 2.

*/
