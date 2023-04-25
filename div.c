// #include "s21_decimal.h"

// int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
//   int k = 0;
//   s21_decimal tmp_1 = value_1;
//   s21_decimal tmp_2 = value_2;
//   s21_decimal reminder = {0};
//   while (s21_is_less_or_equal(tmp_2, tmp_1)) {
//     tmp_2.bits[2] <<= 1;
//     tmp_2.bits[1] <<= 1;
//     tmp_2.bits[0] <<= 1;
//     k++;
//   }
//   tmp_2 = value_2;

//   for (int i = 2; i != -1; i--) {
//     tmp_2.bits[0] <<= k - 1;
//   }
//   *result = tmp_2;

//   s21_sub(tmp_1, tmp_2, &tmp_1);
//   tmp_2 = value_2;

//   while (s21_is_less_or_equal(tmp_2, tmp_1)) {
//     tmp_2.bits[2] <<= 1;
//     tmp_2.bits[1] <<= 1;
//     tmp_2.bits[0] <<= 1;
//     k++;
//   }

//   tmp_2 = value_2;

//   for (int i = 2; i != -1; i--) {
//     tmp_2.bits[0] <<= k - 1;
//   }
// // надо сделать цикл пока value_1 не меньше value_2 чтобы в конце подставить
// reminder и по идее все
//   s21_sub(tmp_1, tmp_2, &tmp_1);

//   return (1);
// }