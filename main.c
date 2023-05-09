#include "s21_decimal.h"

// // int s21_normalize(s21_decimal *num_1, s21_decimal *num_2) {
// //   int power_num_1 = s21_get_power_of_decimal(*num_1);
// //   int power_num_2 = s21_get_power_of_decimal(*num_2);
// //   s21_set_power_of_decimal(num_1, 0);  // ставим степени чисел в 0
// //   s21_set_power_of_decimal(num_2, 0);
// //   s21_decimal bit_number_10 = {10, 0, 0, 0};

// //   if (power_num_1 < power_num_2) {
// //     while (power_num_2 - power_num_1) {
// //       s21_mul(*num_1, bit_number_10, num_1);
// //       power_num_1++;
// //     }
// //     return (power_num_1);
// //   } else if (power_num_2 < power_num_1) {
// //     while (power_num_1 - power_num_2) {
// //       s21_mul(*num_2, bit_number_10, num_2);
// //       power_num_2++;
// //     }
// //     return (power_num_2);
// //   }
// //   return (power_num_1);
// // }

// void s21_print_decimal_number(s21_decimal *num) {
//   int count = 31;
//   long int i = two_32 / 2;   // 2^32
//   short printable_part = 3;  // 0 = 1 strong to print, 1 = 2 ...

//   while (printable_part != -1) {
//     if (count < 23 && count > 14) {
//       printf(" ");
//     }
//     if (num->bits[printable_part] & i && (i == two_32 / 2) &&
//         printable_part == 3) {
//       printf("- ");
//     } else if (!(num->bits[printable_part] & i) && (i == two_32 / 2) &&
//                printable_part == 3) {
//       printf("+ ");
//     } else if (num->bits[printable_part] & i) {
//       printf("%d", 1);
//     } else if (!(num->bits[printable_part] & i) &&
//                !(count < 31 && count > 23) && !(count < 16 && count >= 0)) {
//       printf("%d", 0);
//     }
//     if (i == 1) {
//       printf("\n");
//       printable_part--;
//       i = two_32;
//     }
//     i >>= 1;
//     count--;
//   }
//   printf("\n");
// }

// void s21_print_big_decimal_number(s21_big_decimal *num) {
//   int flg_half_part = 1;
//   int count = 31;
//   long int i = two_32 / 2;  // 2^32
//   short printable_part = 6;

//   while (printable_part != -1) {
//     if (count < 23 && count > 14) {
//       printf(" ");
//     }
//     if (printable_part == 2 && flg_half_part) {
//       printf("--- ordinar decimal part ---\n");
//       flg_half_part--;
//     }
//     if (num->bits[printable_part] & i && (i == two_32 / 2) &&
//         printable_part == 6) {
//       printf("- ");
//     } else if (!(num->bits[printable_part] & i) && (i == two_32 / 2) &&
//                printable_part == 6) {
//       printf("+ ");
//     } else if (num->bits[printable_part] & i) {
//       printf("%d", 1);
//     } else if (!(num->bits[printable_part] & i) &&
//                !(count < 31 && count > 23) && !(count < 16 && count >= 0)) {
//       printf("%d", 0);
//     }
//     if (i == 1) {
//       printf("\n");
//       printable_part--;
//       i = two_32;
//     }
//     i >>= 1;
//     count--;
//   }
//   printf("\n");
// }

// int main(void) {
//   s21_decimal bit_number = {0};
//   s21_decimal bit_number_2 = {0};
//   s21_decimal bit_number_3 = {0};
//   s21_decimal bit_number_result = {0};
//   s21_decimal bit_number_result_2 = {0};
//   int dst = 245;

//   s21_big_decimal big_bit_number = {0};
//   s21_big_decimal big_bit_number_2 = {0};
//   s21_big_decimal big_bit_number_result = {0};
//   s21_big_decimal big_bit_number_result_2 = {0};

//   // ---------работа с int------------

//   // printf("output = %d\n", s21_from_int_to_decimal(112, &bit_number));
//   // printf("output = %d\n", s21_from_decimal_to_int(bit_number, &dst));
//   // printf("dst = %d\n", dst);
//   // s21_from_int_to_decimal(1111111, &bit_number_2);
//   // s21_from_int_to_decimal(2, &bit_number_3);
//   // dst = s21_sub(bit_number, bit_number_2, &bit_number_result);
//   // dst = s21_add(bit_number, bit_number_2, &bit_number_result);

//   // (void)ress;
//   // (void)ress_2;
//   (void)big_bit_number_result_2;
//   (void)big_bit_number_result;
//   (void)big_bit_number;
//   (void)big_bit_number_2;
//   (void)bit_number;
//   (void)bit_number_2;
//   (void)bit_number_3;
//   (void)bit_number_result;
//   (void)bit_number_result_2;
//   (void)dst;

//   // --------работа с deciimal--------

//   // int i = 0;
//   // while (i != 226) {
//   //   if (i <= 95) {
//   //     s21_set_bit_1(&bit_number, i);  // число
//   //   }
//   //   // if (i <= 10) {
//   //   //   s21_set_bit_1(&bit_number_2, i);  // число
//   //   // }
//   //   // // if (i == 127) {
//   //   //   s21_set_bit_1(&bit_number, i);  // знак
//   //   // }
//   //   // if (i == 112) {
//   //   //   s21_set_bit_1(&bit_number_2, i);  // степень
//   //   // }
//   //   i++;
//   // }
//   s21_decimal src1, src2, result;
//   // 30064771176
//   // 3
//   // 10021590392
//   // 10021590392
//   src1.bits[0] = 0b00000000000000000000000001101000;
//   src1.bits[1] = 0b00000000000000000000000000000111;
//   src1.bits[2] = 0b00000000000000000000000000000000;
//   src1.bits[3] = 0b00000000000000000000000000000000;

//   src2.bits[0] = 0b00000000000000000000000000000011;
//   src2.bits[1] = 0b00000000000000000000000000000000;
//   src2.bits[2] = 0b00000000000000000000000000000000;
//   src2.bits[3] = 0b00000000000000000000000000000000;
//   (void)src1;
//   (void)src2;

//   // s21_set_power_of_big_decimal(&big_bit_number_2, 28);
//   // s21_set_power_of_decimal(&bit_number_2, 26);

//   // printf("Output = %d\n", s21_from_float_to_decimal(12, &bit_number));

//   // printf("Output = %d\n", s21_from_float_to_decimal(0.5, &bit_number_2));
//   // printf("децимал перед входом в функцию\n");

//   printf("\nfirst dec number\n");
//   s21_print_decimal_number(&src1);
//   // rewrite_decimal_to_big(&big_bit_number, bit_number);
//   // printf("\nbig_decimal_number first\n");
//   // s21_print_big_decimal_number(&big_bit_number);

//   printf("\nsecond dec number\n");
//   s21_print_decimal_number(&src2);
//   // rewrite_decimal_to_big(&big_bit_number_2, bit_number_2);
//   // printf("big_decimal_number second\n");
//   // s21_print_big_decimal_number(&big_bit_number_2);

//   // s21_truncate_big(big_bit_number, &big_bit_number_result);
//   // s21_round_big(big_bit_number, &big_bit_number_result);

//   // printf("\nbit_res\n");

//   printf("res of func = %d\n", s21_div(src1, src2, &result));
//   //  s21_div_decimal_by_10_big(&big_bit_number));
//   // s21_truncate_big(big_bit_number_result, &big_bit_number_result_2);
//   // printf("%d", count_ones(big_bit_number, big_bit_number_2));

//   // printf("decimal result\n");
//   // s21_print_big_decimal_number(&big_bit_number_result);
//   // printf("decimal result\n");
//   s21_print_decimal_number(&result);

//   // -----------работа с float-----------

//   // float val = 1000;
//   // float result = 0.0;
//   // printf("%.6E", val);
//   // printf("%d\n", rewrite_float_bits_to_buff(&bit_number, i));
//   // printf("Output = %d\n", s21_from_float_to_decimal(val, &bit_number));
//   // printf("децимал перед входом в функцию\n");
//   // s21_print_decimal_number(&bit_number);
//   // printf("децимал после входа в функцию\n");
//   // printf("Output = %d\n", s21_negate(bit_number, &bit_number_result));
//   // s21_print_decimal_number(&bit_number_result);
//   // printf("Power = %d\n", get_power_of_decimal(bit_number));
//   // printf("output = %d\n", s21_from_decimal_to_float(bit_number,
//   // &result)); printf("val = %E\n", val); printf("result = %Ef\n", result);
//   // (void)bit_number;
//   // (void)res;
//   return 0;
// }
