#include "s21_decimal.h"

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

int main(void) {
  s21_decimal bit_number = {0};
  s21_decimal bit_number_2 = {0};
  s21_decimal bit_number_result = {0};
  int dst = 245;

  // ---------работа с int------------

  // printf("%d", get_bit_from_not_decimal(i, 31));
  s21_from_int_to_decimal(2, &bit_number);
  s21_from_int_to_decimal(16, &bit_number_2);
  dst = s21_sub(bit_number, bit_number_2, &bit_number_result);
  // dst = s21_add(bit_number, bit_number_2, &bit_number_result);

  printf("first dec number\n");
  s21_print_decimal_number(&bit_number);
  printf("second dec number\n");
  s21_print_decimal_number(&bit_number_2);
  printf("result dec number\n");
  s21_print_decimal_number(&bit_number_result);
  // // set_dec_number_to_0(&bit_number);
  // print_decimal_number(&bit_number_result);
  // int res_2 = s21_from_decimal_to_int(bit_number_2, &dst_2);

  // (void)ress;
  // (void)ress_2;
  // (void)bit_number;
  (void)bit_number_2;
  (void)bit_number_result;
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

  // int i = 0;
  // while (i != 128) {
  //   if (i == 31 || i == 0) {
  //     s21_set_bit_1(&bit_number, i);
  //   }
  //   if (i == 127) {
  //     s21_set_bit_1(&bit_number, i);  // знак
  //   }
  //   if (i == 112 || i == 115) {
  //     s21_set_bit_1(&bit_number, i);  // степень
  //   }
  //   i++;
  // }
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

  // float val = 10.0;
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

/* dashboard
  - добавить обработку на null inf -inf nan (сделано для float_to_decimal)
  - написать negeate

  done
  - add comparence to substruction

*/