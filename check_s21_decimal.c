#include <check.h>
#include <limits.h>
#include <stdlib.h>

#include "s21_decimal.h"

int last_bits(int pow,  // первый аргумент заполняет степень, второй аргумент
                        // знак (0 или 1) для последнего бита массива
              int sign) {  // Возвращает число для последнего бита массива
  return (pow << 16) ^ (sign << 31);
}

float s21_rand_r(float a, float b) {  //  генерирует случайное число с плавающей
                                      //  точкой в диапазоне от a до b.
  float m = (float)rand() / RAND_MAX;
  float num = a + m * (b - a);
  return num;
}

void s21_set_bit(s21_decimal *dst, int index, int bit) {
  int mask = 1u << (index % 32);
  dst->bits[index / 32] =
      bit == 0 ? dst->bits[index / 32] & ~mask : dst->bits[index / 32] | mask;
}

int great_bez_znak(s21_decimal a, s21_decimal b) {
  int znak1 = s21_get_bits(a.bits[3], 31);
  int znak2 = s21_get_bits(b.bits[3], 31);
  s21_set_bit(&a, 127, 0);
  s21_set_bit(&b, 127, 0);
  int res = s21_is_greater(a, b);
  s21_set_bit(&a, 127, znak1);
  s21_set_bit(&b, 127, znak2);
  return res;
}

// вывести весь децималал
// void s21_print_bits(s21_decimal *dst) {
//   printf("\n");
//   for (int i = 0; i <= 3; i++) {
//     for (int bit = 0; bit <= 31; ++bit) {
//       printf("%d", (dst->bits[i] >> bit) & 1);
//     }
//     printf(" ");
//   }
//   printf("\n\n");
// }

//--------------s21_from_int_to_decimal----------------//

START_TEST(test_s21_from_int_to_decimal_1) {
  s21_decimal dec;
  int ret = s21_from_int_to_decimal(12345, &dec);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(dec.bits[0], 12345);
  ck_assert_int_eq(dec.bits[1], 0);
  ck_assert_int_eq(dec.bits[2], 0);
  ck_assert_int_eq(dec.bits[3], 0);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_2) {
  s21_decimal dec;
  s21_set_dec_number_to_0(&dec);
  int ret = s21_from_int_to_decimal(-12345, &dec);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(dec.bits[0], 12345);
  ck_assert_int_eq(dec.bits[1], 0);
  ck_assert_int_eq(dec.bits[2], 0);
  ck_assert_int_eq(dec.bits[3], 0x80000000);
}
END_TEST

START_TEST(test_s21_from_int_to_decimal_3) {
  s21_decimal dec;
  int ret = s21_from_int_to_decimal(0, &dec);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(dec.bits[0], 0);
  ck_assert_int_eq(dec.bits[1], 0);
  ck_assert_int_eq(dec.bits[2], 0);
  ck_assert_int_eq(dec.bits[3], 0);
}
END_TEST

START_TEST(from_int_to_decimal) {
  int x = 555655555;
  s21_decimal y;
  s21_from_int_to_decimal(x, &y);
  ck_assert_int_eq(abs(x), y.bits[0]);
  ck_assert_int_eq(0, y.bits[3]);
}
END_TEST

START_TEST(from_int_to_decimal2) {
  int x = -555655555;
  s21_decimal y;
  s21_from_int_to_decimal(x, &y);
  ck_assert_int_eq(abs(x), y.bits[0]);
  ck_assert_int_eq(2147483648, y.bits[3]);
}
END_TEST

START_TEST(from_int_to_decimal3) {
  unsigned int x = 193939393;
  s21_decimal y;
  ck_assert_int_eq(0, s21_from_int_to_decimal(x, &y));
}
END_TEST

//---------------s21_from_decimal_to_int-----------------------//

START_TEST(s21_from_decimal_to_int_1) {
  s21_decimal dec = {{235, 0, 0, 0}};
  int x = 0;
  ;
  int ret = s21_from_decimal_to_int(dec, &x);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(x, 235);
}
END_TEST

START_TEST(s21_from_decimal_to_int_2) {
  s21_decimal dec = {{235, 0, 0, 0x80000000}};
  int x = 0;
  int ret = s21_from_decimal_to_int(dec, &x);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(x, -235);
}
END_TEST

START_TEST(s21_from_decimal_to_int_3) {
  s21_decimal dec = {{235, 235, 0, 0}};
  int x = 0;
  int ret = s21_from_decimal_to_int(dec, &x);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_from_decimal_to_int_4) {
  s21_decimal dec = {{4294967294, 0, 0, 0}};
  int x = 0;
  int ret = s21_from_decimal_to_int(dec, &x);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(s21_from_decimal_to_int_5) {
  s21_decimal dec = {{2147483647, 0, 0, 0}};
  int x = 0;
  int ret = s21_from_decimal_to_int(dec, &x);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(x, 2147483647);
}
END_TEST

START_TEST(s21_from_decimal_to_int_6) {
  s21_decimal dec = {{2147483647, 0, 0, 0x80000000}};
  int x = 0;
  int ret = s21_from_decimal_to_int(dec, &x);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(x, -2147483647);
}
END_TEST

START_TEST(s21_from_decimal_to_int_7) {
  s21_decimal dec = {{123456789, 0, 0, 0x80000000}};
  int x = 0;
  int ret = s21_from_decimal_to_int(dec, &x);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(x, -123456789);
}
END_TEST

START_TEST(from_decimal_to_int) {
  s21_decimal x = {{0, 0, 0, 0}};
  int y;
  s21_from_decimal_to_int(x, &y);
  ck_assert_int_eq(0, y);
}
END_TEST

START_TEST(from_decimal_to_int2) {
  s21_decimal x = {{INT_MAX, 0, 0, 2147483648}};
  int y;
  s21_from_decimal_to_int(x, &y);
  ck_assert_int_eq(-INT_MAX, y);
}
END_TEST

//--------------------------s21_add-------------------------------//

START_TEST(test_add) {
  s21_decimal one = {{10, 0, 0, 0}};
  s21_decimal two = {{10, 0, 0, 0}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 20);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_add2) {
  s21_decimal one = {{2147483647, 0, 0, 0}};
  s21_decimal two = {{2147483647, 0, 0, 0}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 4294967294);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_add3) {
  s21_decimal one = {{2147483649, 0, 0, 0}};
  s21_decimal two = {{2147483649, 0, 0, 0}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 2);
  ck_assert_int_eq(res.bits[1], 1);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_add4) {
  s21_decimal one = {{4294967295}};
  s21_decimal two = {{4294967295}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 4294967294);
  ck_assert_int_eq(res.bits[1], 1);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_add5) {
  s21_decimal one = {{2147483649}};
  s21_decimal two = {{2147483649}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 2);
  ck_assert_int_eq(res.bits[1], 1);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_add6) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295}};
  s21_decimal two = {{4294967295, 4294967295, 4294967295}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(ret, 1);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_add7) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295}};
  s21_decimal two = {{1, 0, 0}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(ret, 1);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_add8) {
  s21_decimal one = {{15, 0, 0}};
  s21_decimal two = {{30, 0, 0}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 45);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(add1) {
  s21_decimal one = {{155, 0, 0, last_bits(1, 0)}};
  s21_decimal two = {{155, 0, 0, last_bits(1, 0)}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 310);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], last_bits(1, 0));
}
END_TEST

START_TEST(add5) {
  s21_decimal one = {{152, 0, 0, last_bits(1, 0)}};
  s21_decimal two = {{153, 0, 0, last_bits(1, 0)}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 305);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], last_bits(1, 0));
}
END_TEST

START_TEST(add2) {
  s21_decimal one = {{156, 0, 0, last_bits(1, 0)}};
  s21_decimal two = {{155, 0, 0, last_bits(1, 0)}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 311);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], last_bits(1, 0));
}
END_TEST

START_TEST(add3) {
  s21_decimal one = {{159, 0, 0, last_bits(1, 0)}};
  s21_decimal two = {{158, 0, 0, last_bits(1, 0)}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 317);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], last_bits(1, 0));
}
END_TEST

START_TEST(add4) {
  s21_decimal one = {{15123, 0, 0, last_bits(3, 0)}};
  s21_decimal two = {{15111, 0, 0, last_bits(3, 0)}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 30234);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], last_bits(3, 0));
}
END_TEST

START_TEST(add6) {
  s21_decimal one = {{15123, 0, 0, last_bits(3, 0)}};
  s21_decimal two = {{1511, 0, 0, last_bits(2, 0)}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 30233);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], last_bits(3, 0));
}
END_TEST

START_TEST(add7) {
  s21_decimal one = {{15123, 0, 0, last_bits(2, 0)}};
  s21_decimal two = {{15111, 0, 0, last_bits(4, 0)}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 1527411);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], last_bits(4, 0));
}
END_TEST

START_TEST(add8) {
  s21_decimal one = {{15123, 0, 0, last_bits(4, 0)}};
  s21_decimal two = {{15111, 0, 0, last_bits(2, 0)}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 1526223);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], last_bits(4, 0));
}
END_TEST

START_TEST(add9) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, last_bits(0, 0)}};
  s21_decimal two = {{5, 0, 0, last_bits(1, 0)}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 4294967295);
  ck_assert_int_eq(res.bits[1], 4294967295);
  ck_assert_int_eq(res.bits[2], 4294967295);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(add12) {
  s21_decimal one = {{10, 0, 0, last_bits(0, 1)}};
  s21_decimal two = {{12, 0, 0, last_bits(0, 0)}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(res.bits[0], 2);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(add13) {
  s21_decimal one = {{429496729, 0, 0, last_bits(0, 0)}};
  s21_decimal two = {{3865470566, 0, 0, last_bits(0, 0)}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(res.bits[0], 4294967295);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(add14) {
  s21_decimal one = {{4294967295, 4294967295, 429496729, last_bits(0, 0)}};
  s21_decimal two = {{0, 0, 3865470566, last_bits(0, 0)}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(res.bits[0], 4294967295);
  ck_assert_int_eq(res.bits[1], 4294967295);
  ck_assert_int_eq(res.bits[2], 4294967295);
  ck_assert_int_eq(res.bits[3], 0);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(add15) {
  s21_decimal one = {{4294967295, 429496729, 0, last_bits(0, 0)}};
  s21_decimal two = {{0, 3865470566, 0, last_bits(0, 0)}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(res.bits[0], 4294967295);
  ck_assert_int_eq(res.bits[1], 4294967295);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(add16) {
  s21_decimal one = {{4294967295, 4294967295, 429496729, last_bits(0, 0)}};
  s21_decimal two = {{0, 0, 3865470566, last_bits(0, 0)}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(res.bits[0], 4294967295);
  ck_assert_int_eq(res.bits[1], 4294967295);
  ck_assert_int_eq(res.bits[2], 4294967295);
  ck_assert_int_eq(res.bits[3], 0);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(add17) {
  s21_decimal one = {{10, 0, 0, last_bits(0, 1)}};
  s21_decimal two = {{8, 0, 0, last_bits(0, 0)}};
  s21_decimal res = {{0}};
  int ret = s21_add(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 2);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 2147483648);
}
END_TEST

START_TEST(s21_add_test_1) {
  s21_decimal a = {{4294967295, 4294967295, 4294967295,
                    0}};                    //   79228162514264337593543950335
  s21_decimal b = {{1, 0, 0, 2147483648}};  //   -1
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(res_bits.bits[0], 4294967294);
  ck_assert_int_eq(res_bits.bits[1], 4294967295);
  ck_assert_int_eq(res_bits.bits[2], 4294967295);
  ck_assert_int_eq(res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_add_test_2) {
  s21_decimal a = {{4294967295, 47083, 0, 65536}};  // 20222222222222.9
  s21_decimal b = {{0, 0, 0, 0}};                   // 0
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);

  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(res_bits.bits[0], 4294967295);
  ck_assert_int_eq(res_bits.bits[1], 47083);
  ck_assert_int_eq(res_bits.bits[2], 0);
  ck_assert_int_eq(res_bits.bits[3], 65536);
}
END_TEST

START_TEST(s21_add_test_3) {
  s21_decimal a = {{292342, 0, 0, 327680}};  // 2.92342
  s21_decimal b = {{0, 0, 0, 0}};            // 0
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 292342);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 327680);
}
END_TEST

START_TEST(s21_add_test_4) {
  s21_decimal a = {{292342, 0, 0, last_bits(5, 1)}};  // -2.92342
  s21_decimal b = {{781413, 0, 0, last_bits(0, 1)}};  // -781413
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(res_bits.bits[0], 832181014);
  ck_assert_int_eq(res_bits.bits[1], 18);
  ck_assert_int_eq(res_bits.bits[2], 0);
  ck_assert_int_eq(res_bits.bits[3], 2147811328);
}
END_TEST

START_TEST(s21_add_test_5) {
  s21_decimal a = {{-1158315456, -197194979, 33, 0}};  //  626342352523521000000
  s21_decimal b = {{781413, 0, 0, -2147483648}};       //   -781413
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -1159096869);
  ck_assert_int_eq((int)res_bits.bits[1], -197194979);
  ck_assert_int_eq((int)res_bits.bits[2], 33);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_add_test_6) {
  s21_decimal a = {{4294967295, 4294967295, 4294967295,
                    last_bits(0, 1)}};  //   -79228162514264337593543950335
  s21_decimal b = {{4294967295, 4294967295, 4294967295,
                    0}};  //   79228162514264337593543950335
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 0);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_add_test_7) {
  s21_decimal a = {{4294967295, 4294967295, 4294967295,
                    0}};  //   79228162514264337593543950335
  s21_decimal b = {{4294967295, 4294967295, 4294967295,
                    0}};  // 79228162514264337593543950335
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq(res, 1);
  ck_assert_int_eq(res_bits.bits[0], 0);
  ck_assert_int_eq(res_bits.bits[1], 0);
  ck_assert_int_eq(res_bits.bits[2], 0);
  ck_assert_int_eq(res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_add_test_8) {
  s21_decimal a = {{1015837740, 287445, 0, 0}};  // 1234567890236460
  s21_decimal b = {{-2045900063, 28744, 0, 0}};  // 123456789023457
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -1030062323);
  ck_assert_int_eq((int)res_bits.bits[1], 316189);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_add_test_9) {
  s21_decimal a = {{-1345409089, 146074521, 0, 0}};  // 627385293423423423
  s21_decimal b = {
      {1668895137, 1064696021, 0, -2147483648}};  // - 4572834592045224353
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -1280663070);
  ck_assert_int_eq((int)res_bits.bits[1], 918621499);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_add_test_10) {
  s21_decimal a = {{5, 0, 0, -2147483648}};  // -5
  s21_decimal b = {{0, 0, 0, 0}};            // 0
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 5);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_add_test_11) {
  s21_decimal a = {{351661334, 63675, 0, 0}};  // 273482394234134
  s21_decimal b = {{7, 0, 0, 0}};              // 7
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 351661341);
  ck_assert_int_eq((int)res_bits.bits[1], 63675);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_add_test_12) {
  s21_decimal a = {{61237812, 0, 0, -2147024896}};  // -6.1237812
  s21_decimal b = {{81230311, 0, 0, 196608}};       // 81230.311
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 493053244);
  ck_assert_int_eq((int)res_bits.bits[1], 189);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 458752);
}
END_TEST

START_TEST(s21_add_test_13) {
  s21_decimal a = {{17403, 0, 0, 0}};              // 17403
  s21_decimal b = {{4789282, 0, 0, -2147352576}};  // -47892.82
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 3048982);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147352576);
}
END_TEST

START_TEST(s21_add_test_14) {
  s21_decimal a = {{72342342, 0, 0, 524288}};  // 0.72342342
  s21_decimal b = {{2, 0, 0, 1703936}};        //   0.00000000000000000000000002
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 1364721666);
  ck_assert_int_eq((int)res_bits.bits[1], 936107310);
  ck_assert_int_eq((int)res_bits.bits[2], 3921686);
  ck_assert_int_eq((int)res_bits.bits[3], 1703936);
}
END_TEST

START_TEST(s21_add_test_15) {
  s21_decimal a = {{610781734, 168435, 0, 983040}};  // 0.723423427283494
  s21_decimal b = {{784, 0, 0, 196608}};             // 0.784
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 1575537190);
  ck_assert_int_eq((int)res_bits.bits[1], 350974);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 983040);
}
END_TEST

START_TEST(s21_add_test_16) {
  s21_decimal a = {{7938402, 0, 0, 0}};               // 7938402
  s21_decimal b = {{1830490002, 0, 0, -2147155968}};  // -18304.90002
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 1735727534);
  ck_assert_int_eq((int)res_bits.bits[1], 184);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 327680);
}
END_TEST

START_TEST(s21_add_test_17) {
  s21_decimal a = {{2024072662, 184830, 0, 589824}};  // 793840.829392342
  s21_decimal b = {{1265466682, 55512, 0, 524288}};   // 2384234.90002234
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 1793837594);
  ck_assert_int_eq((int)res_bits.bits[1], 739953);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 589824);
}
END_TEST

START_TEST(s21_add_test_18) {
  s21_decimal a = {
      {809805404, 16957601, 0, -2147483648}};         // -72832342523422300
  s21_decimal b = {{-1998155945, 28268, 0, 196608}};  // 121412432334.679
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 54741001);
  ck_assert_int_eq((int)res_bits.bits[1], -222296264);
  ck_assert_int_eq((int)res_bits.bits[2], 3);
  ck_assert_int_eq((int)res_bits.bits[3], -2147287040);
}
END_TEST

START_TEST(s21_add_test_19) {
  s21_decimal a = {
      {809805404, 16957601, 0, -2147483648}};  //   -72832342523422300
  s21_decimal b = {{652204261, 150968, 0, -2147221504}};  //   -64840327494.6789
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -1602075995);
  ck_assert_int_eq((int)res_bits.bits[1], 2072438309);
  ck_assert_int_eq((int)res_bits.bits[2], 39);
  ck_assert_int_eq((int)res_bits.bits[3], -2147221504);
}
END_TEST

START_TEST(s21_add_test_20) {
  s21_decimal a = {{-1318475665, 151177, 0, 0}};          // 649303247399023
  s21_decimal b = {{652206196, 150968, 0, -2147221504}};  //   -64840327494.8724
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 140742524);
  ck_assert_int_eq((int)res_bits.bits[1], 1511625962);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 262144);
}
END_TEST

START_TEST(s21_add_test_21) {
  s21_decimal a = {{0, 0, 0, 0}};  // 0
  s21_decimal b = {{0, 0, 0, 0}};  // 0
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 0);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_add_test_22) {
  s21_decimal a = {{292342, 0, 0, -2147155968}};  // -2.92342
  s21_decimal b = {{781413, 0, 0, -2147483648}};  // -781413
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 832181014);
  ck_assert_int_eq((int)res_bits.bits[1], 18);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147155968);
}
END_TEST

START_TEST(s21_add_test_23) {
  s21_decimal a = {{999999999, 0, 0, 0}};  // 999999999
  s21_decimal b = {{999999999, 0, 0, 0}};  // 999999999
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 1999999998);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_add_test_24) {
  s21_decimal a = {{-1, -1, -78293423, 0}};  // 77783903795975396563441680383
  s21_decimal b = {{-2147483648, -2147483648, 628902378,
                    0}};  // 11601201223536716331618402304
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 1);
  ck_assert_int_eq((int)res_bits.bits[0], 0);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_add_test_25) {
  s21_decimal a = {
      {-1, -1, -78293423, -2147483648}};  // -77783903795975396563441680383
  s21_decimal b = {{-2147483648, -2147483648, 628902378,
                    -2147483648}};  // -11601201223536716331618402304
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 2);
  ck_assert_int_eq((int)res_bits.bits[0], 0);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_add_test_26) {
  s21_decimal a = {{1, 0, 0, -2147483648}};  // -1
  s21_decimal b = {
      {-1, -1, -1, -2147483648}};  // -79228162514264337593543950335
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 2);
  ck_assert_int_eq((int)res_bits.bits[0], 0);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_add_test_29) {
  s21_decimal a = {{8, 0, 0, 0}};            // 8
  s21_decimal b = {{1, 0, 0, -2147483648}};  // -1
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 7);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_add_test_30) {
  s21_decimal a = {{10, 0, 0, 0}};           // 10
  s21_decimal b = {{1, 0, 0, -2147483648}};  // -1
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 9);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_add_test_31) {
  s21_decimal a = {{1, 0, 0, -2147483648}};  // -1
  s21_decimal b = {{-1, -1, -1, 0}};         // 79228162514264337593543950335
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -2);
  ck_assert_int_eq((int)res_bits.bits[1], -1);
  ck_assert_int_eq((int)res_bits.bits[2], -1);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_add_test_32) {
  s21_decimal a = {{1, 0, 0, 0}};  // 1
  s21_decimal b = {
      {-1, -1, -1, -2147483648}};  // -79228162514264337593543950335
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -2);
  ck_assert_int_eq((int)res_bits.bits[1], -1);
  ck_assert_int_eq((int)res_bits.bits[2], -1);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_add_test_33) {
  s21_decimal a = {{-1, -1, -1, 0}};   // 79228162514264337593543950335
  s21_decimal b = {{6, 0, 0, 65536}};  // 0.6
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 1);
  ck_assert_int_eq((int)res_bits.bits[0], 0);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_add_test_34) {
  s21_decimal a = {
      {-1, -1, -1, -2147483648}};            //   -79228162514264337593543950335
  s21_decimal b = {{6, 0, 0, -2147418112}};  // -0.6
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 2);
  ck_assert_int_eq((int)res_bits.bits[0], 0);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_add_test_35) {
  s21_decimal a = {{-1, -1, -1, 0}};         // 79228162514264337593543950335
  s21_decimal b = {{6, 0, 0, -2147418112}};  // -0.6
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -2);
  ck_assert_int_eq((int)res_bits.bits[1], -1);
  ck_assert_int_eq((int)res_bits.bits[2], -1);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_add_test_36) {
  s21_decimal a = {
      {-1, -1, -1, -2147483648}};      // -79228162514264337593543950335
  s21_decimal b = {{6, 0, 0, 65536}};  // 0.6
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -2);
  ck_assert_int_eq((int)res_bits.bits[1], -1);
  ck_assert_int_eq((int)res_bits.bits[2], -1);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_add_test_37) {
  s21_decimal a = {{-1, -1, -1, 0}};    // 79228162514264337593543950335
  s21_decimal b = {{1, 0, 0, 196608}};  // 0.001
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_add(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -1);
  ck_assert_int_eq((int)res_bits.bits[1], -1);
  ck_assert_int_eq((int)res_bits.bits[2], -1);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

//---------------------------s21_sub---------------------------//

START_TEST(test_sub1) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 0}};
  s21_decimal two = {{4294967295, 4294967295, 4294967295, 0}};
  s21_decimal res = {{0}};
  int ret = s21_sub(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_sub2) {
  s21_decimal one = {{4294967295, 0, 0}};
  s21_decimal two = {{1124417, 0, 0}};
  s21_decimal res = {{0}};
  int ret = s21_sub(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 4293842878);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_sub3) {
  s21_decimal one = {{4294967295, 4294967295, 0}};
  s21_decimal two = {{1124417, 0, 0}};
  s21_decimal res = {{0}};
  int ret = s21_sub(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 4293842878);
  ck_assert_int_eq(res.bits[1], 4294967295);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_sub4) {
  s21_decimal one = {{100, 0, 0, 0}};
  s21_decimal two = {{200, 0, 0, 0}};
  s21_decimal res = {{0}};
  int ret = s21_sub(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 100);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0x80000000);
}
END_TEST

START_TEST(test_sub5) {
  s21_decimal one = {{4294967295, 4294967295, 0, 0}};
  s21_decimal two = {{3152150271, 3690724337, 0, 0}};
  s21_decimal res = {{0}};
  int ret = s21_sub(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 1142817024);
  ck_assert_int_eq(res.bits[1], 604242958);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_sub6) {
  s21_decimal one = {{8, 0, 0, 0}};
  s21_decimal two = {{1, 0, 0, 0}};
  s21_decimal res = {{0}};
  int ret = s21_sub(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 7);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_sub7) {
  s21_decimal one = {{123456, 0, 0, last_bits(1, 0)}};
  s21_decimal two = {{123456, 0, 0, last_bits(3, 0)}};
  s21_decimal res = {{0}};
  int ret = s21_sub(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 12222144);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], last_bits(3, 0));
}
END_TEST

START_TEST(test_sub8) {
  s21_decimal one = {{12345600, 0, 0, 0}};
  s21_decimal two = {{123456, 0, 0, 0}};
  s21_decimal res = {{0}};
  int ret = s21_sub(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 12222144);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_sub9) {
  s21_decimal one = {{12345656, 0, 0, last_bits(3, 0)}};
  s21_decimal two = {{123456789, 0, 0, last_bits(8, 0)}};
  s21_decimal res = {{0}};
  int ret = s21_sub(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 1786529259);
  ck_assert_int_eq(res.bits[1], 287);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], last_bits(8, 0));
}
END_TEST

START_TEST(test_sub10) {
  s21_decimal one = {{10, 0, 0, last_bits(0, 1)}};
  s21_decimal two = {{5, 0, 0, last_bits(0, 0)}};
  s21_decimal res = {{0}};
  int ret = s21_sub(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 15);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 2147483648);
}
END_TEST

START_TEST(test_sub11) {
  s21_decimal one = {{12345678, 0, 0, last_bits(3, 1)}};
  s21_decimal two = {{123456789, 0, 0, last_bits(5, 0)}};
  s21_decimal res = {{0}};
  int ret = s21_sub(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 1358024589);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 2147811328);
}
END_TEST

START_TEST(test_sub12) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, last_bits(0, 1)}};
  s21_decimal two = {{1, 0, 0, last_bits(1, 0)}};
  s21_decimal res = {{0}};
  int ret = s21_sub(one, two, &res);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(test_sub14) {
  s21_decimal one = {{12345, 0, 0, last_bits(2, 1)}};
  s21_decimal two = {{12345, 0, 0, last_bits(1, 1)}};
  s21_decimal res = {{0}};
  int ret = s21_sub(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 111105);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 2147614720);
}
END_TEST

START_TEST(test_sub15) {
  s21_decimal one = {{10, 0, 0, last_bits(0, 1)}};
  s21_decimal two = {{12, 0, 0, last_bits(0, 1)}};
  s21_decimal res = {{0}};
  int ret = s21_sub(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 2);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_sub16) {
  s21_decimal one = {{10, 0, 0, last_bits(0, 1)}};
  s21_decimal two = {{8, 0, 0, last_bits(0, 1)}};
  s21_decimal res = {{0}};
  int ret = s21_sub(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 2);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 2147483648);
}
END_TEST

START_TEST(test_sub17) {
  s21_decimal one = {{1026323, 0, 0, last_bits(5, 1)}};
  s21_decimal two = {{823123, 0, 0, last_bits(5, 1)}};
  s21_decimal res = {{0}};
  int ret = s21_sub(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 203200);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 2147811328);
}
END_TEST

START_TEST(test_sub18) {
  s21_decimal one = {{10, 0, 0, last_bits(0, 1)}};
  s21_decimal two = {{8, 0, 0, last_bits(0, 0)}};
  s21_decimal res = {{0}};
  int ret = s21_sub(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 18);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 2147483648);
}
END_TEST

START_TEST(test_sub19) {
  s21_decimal a = {{17403, 0, 0, 0}};                  // 17403
  s21_decimal b = {{4789282, 0, 0, last_bits(2, 0)}};  // 47892.82
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_sub(a, b, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 3048982);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], last_bits(2, 1));
}
END_TEST

START_TEST(test_sub20) {
  s21_decimal a = {{4294967295, 4294967295, 4294967295,
                    0}};  // 79,228,162,514,264,337,593,543,950,335
  s21_decimal b = {{6, 0, 0, last_bits(1, 0)}};  // 0.6
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_sub(a, b, &res_bits);
  ck_assert_int_eq(res, 0);
  ck_assert_int_eq(res_bits.bits[0], 4294967294);
  ck_assert_int_eq(res_bits.bits[1], 4294967295);
  ck_assert_int_eq(res_bits.bits[2], 4294967295);
  ck_assert_int_eq(res_bits.bits[3], 0);
}
END_TEST

//-------------------------s21_mul------------------------//

START_TEST(test_mul1) {
  s21_decimal one = {{5, 0, 0, 0}};
  s21_decimal two = {{3, 0, 0, 0}};
  s21_decimal res = {{0}};
  int ret = s21_mul(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 15);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_mul2) {
  s21_decimal one = {{15, 0, 0, 0}};
  s21_decimal two = {{15, 0, 0, 0}};
  s21_decimal res = {{0}};
  int ret = s21_mul(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 225);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_mul3) {
  s21_decimal one = {{4294967295, 0, 0, 0}};
  s21_decimal two = {{32, 0, 0, 0}};
  s21_decimal res = {{0}};
  int ret = s21_mul(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 4294967264);
  ck_assert_int_eq(res.bits[1], 31);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_mul4) {
  s21_decimal one = {{4294967295, 4294967295, 0, 0}};
  s21_decimal two = {{4294967295, 32, 0, 0}};
  s21_decimal res = {{0}};
  int ret = s21_mul(one, two, &res);
  ck_assert_int_eq(ret, 1);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_mul5) {
  s21_decimal one = {{1, 0, 0, 0}};
  s21_decimal two = {{10, 0, 0, 0}};
  s21_decimal res = {{0}};
  int ret = s21_mul(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 10);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_mul6) {
  s21_decimal one = {{2, 0, 0, 0}};
  s21_decimal two = {{100, 0, 0, 0}};
  s21_decimal res = {{0}};
  int ret = s21_mul(one, two, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 200);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(s21_mul_test_1) {
  s21_decimal a = {{-1, -1, -1, 0}};         // 79228162514264337593543950335
  s21_decimal b = {{1, 0, 0, -2147483648}};  // -1
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -1);
  ck_assert_int_eq((int)res_bits.bits[1], -1);
  ck_assert_int_eq((int)res_bits.bits[2], -1);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_mul_test_2) {
  s21_decimal a = {{-2017942635, 47083, 0, 65536}};  // 20222222222222.9
  s21_decimal b = {{0, 0, 0, 0}};                    // 0
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 0);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_mul_test_3) {
  s21_decimal a = {{292342, 0, 0, 327680}};  // 2.92342
  s21_decimal b = {{0, 0, 0, 0}};            // 0
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 0);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_mul_test_4) {
  s21_decimal a = {{292342, 0, 0, -2147155968}};  // -2.92342
  s21_decimal b = {{781413, 0, 0, -2147483648}};  // -781413
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 806572558);
  ck_assert_int_eq((int)res_bits.bits[1], 53);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 327680);
}
END_TEST

START_TEST(s21_mul_test_5) {
  s21_decimal a = {{-1158315456, -197194979, 33, 0}};  // 626342352523521000000
  s21_decimal b = {{781413, 0, 0, -2147483648}};       // -781413
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -1347460288);
  ck_assert_int_eq((int)res_bits.bits[1], -177876063);
  ck_assert_int_eq((int)res_bits.bits[2], 26532164);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

// Value was either too large or too small for a Decimal.
START_TEST(s21_mul_test_6) {
  s21_decimal a = {
      {-1, -1, -1, -2147483648}};     // -79228162514264337593543950335
  s21_decimal b = {{-1, -1, -1, 0}};  // 79228162514264337593543950335
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 2);
  ck_assert_int_eq((int)res_bits.bits[0], 0);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

// Value was either too large or too small for a Decimal.
START_TEST(s21_mul_test_7) {
  s21_decimal a = {{-1, -1, -1, 0}};  // 79228162514264337593543950335
  s21_decimal b = {{-1, -1, -1, 0}};  // 79228162514264337593543950335
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 1);
  ck_assert_int_eq((int)res_bits.bits[0], 0);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

// Value was either too large or too small for a Decimal.
START_TEST(s21_mul_test_8) {
  s21_decimal a = {{1015837740, 287445, 0, 0}};  // 1234567890236460
  s21_decimal b = {{-2045900063, 28744, 0, 0}};  // 123456789023457
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);
  // s21_print_bits(&res_bits);
  ck_assert_int_eq((int)res, 1);
  ck_assert_int_eq((int)res_bits.bits[0], 0);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(mul_0) {
  s21_decimal val1 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{2, 0, 0, 0}};
  s21_decimal res = {{0}};
  ck_assert_int_eq(0, s21_mul(val1, val2, &res));
}
END_TEST

START_TEST(mul_1) {
  s21_decimal val1 = {{2, 0, 0, 0}};
  s21_decimal val2 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal res;
  ck_assert_int_eq(0, s21_mul(val1, val2, &res));
}
END_TEST

START_TEST(mul_2) {
  s21_decimal val1 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal res = {{0}};
  ck_assert_int_eq(0, s21_mul(val1, val2, &res));
}
END_TEST

START_TEST(mul_3) {
  s21_decimal val1 = {{2, 0, 0, 0}};
  s21_decimal val2 = {{2, 0, 0, 0}};
  s21_decimal res = {{0}};
  ck_assert_int_eq(0, s21_mul(val1, val2, &res));
}
END_TEST

START_TEST(mul_4) {
  s21_decimal val1 = {{8, 0, 0, 0}};
  s21_decimal val2 = {{2, 0, 0, 0}};
  s21_decimal res = {{0}};
  ck_assert_int_eq(0, s21_mul(val1, val2, &res));
}
END_TEST

START_TEST(mul_5) {
  s21_decimal val1 = {{2, 0, 0, 0}};
  s21_decimal val2 = {{8, 0, 0, 0}};
  s21_decimal res = {{0}};
  ck_assert_int_eq(0, s21_mul(val1, val2, &res));
}
END_TEST

START_TEST(mul_6) {
  s21_decimal val1 = {{8, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{2, 0, 0, 0}};
  s21_decimal res = {{0}};
  ck_assert_int_eq(0, s21_mul(val1, val2, &res));
}
END_TEST

START_TEST(mul_7) {
  s21_decimal val1 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{8, 0, 0, 0}};
  s21_decimal res = {{0}};
  ck_assert_int_eq(0, s21_mul(val1, val2, &res));
}
END_TEST

START_TEST(mul_8) {
  s21_decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal res = {{0}};
  ck_assert_int_eq(1, s21_mul(val1, val2, &res));
}
END_TEST

START_TEST(mul_9) {
  s21_decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{2, 0, 0, 0}};
  s21_decimal res = {{0}};
  ck_assert_int_eq(2, s21_mul(val1, val2, &res));
}
END_TEST

START_TEST(mul_10) {
  s21_decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, 0}};
  s21_decimal val2 = {{2, 0, 0, 0}};
  s21_decimal res = {{0}};
  ck_assert_int_eq(1, s21_mul(val1, val2, &res));
}
END_TEST

START_TEST(mul_11) {
  s21_decimal val1 = {{UINT_MAX, UINT_MAX, UINT_MAX, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{0, 0, 0, 0}};
  s21_decimal res = {{0}};
  ck_assert_int_eq(0, s21_mul(val1, val2, &res));
}
END_TEST

START_TEST(mul_12) {
  s21_decimal value_1 = {{5, 0, 0, 0}};
  s21_decimal value_2 = {{7, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal check = {{35, 0, 0, 0}};
  int return_value = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(mul_13) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0, 0, 0}};
  s21_decimal value_2 = {{0xFFFFFFFF, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal check = {{1, 0xFFFFFFFE, 0, 0}};
  int return_value = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(mul_14) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{2, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal check = {{0, 0, 0, 0}};
  int return_value = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 1);
}
END_TEST

START_TEST(mul_15) {
  s21_decimal value_1 = {{123456u, 123u, 0, 0}};
  s21_decimal value_2 = {{654321u, 654u, 0, 0}};
  s21_set_bit_1(&value_2, 127);
  s21_set_power_of_decimal(&value_1, 2);
  s21_set_power_of_decimal(&value_2, 3);
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal check = {{0xcedabe40, 0x99c0c5d, 0x13a3a, 0x80050000}};
  int return_value = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(mul_16) {
  s21_decimal value_1 = {{0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0}};
  s21_decimal value_2 = {{2, 0, 0, 0}};
  s21_set_bit_1(&value_2, 127);
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal check = {{0, 0, 0, 0}};
  int return_value = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 2);
}
END_TEST

START_TEST(mul_17) {
  s21_decimal value_1 = {{17, 0, 0, 0}};
  s21_set_bit_1(&value_1, 127);
  s21_decimal value_2 = {{0, 0, 0, 0}};
  s21_decimal result = {0};
  s21_decimal check = {{0, 0, 0, 0}};
  int return_value = s21_mul(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(mul_18) {
  s21_decimal src1, src2;
  float a = 9403.0e2;
  float b = 9403.0e2;
  float res_our_dec = 0.0;
  s21_from_float_to_decimal(a, &src1);
  s21_from_float_to_decimal(b, &src2);
  float res_origin = 884164090000;
  s21_decimal res_od = {0};
  s21_mul(src1, src2, &res_od);
  s21_from_decimal_to_float(res_od, &res_our_dec);
  ck_assert_float_eq(res_our_dec, res_origin);
}
END_TEST

START_TEST(mul_test_1) {
  int num1 = -10;
  int num2 = -10;
  int prod_int = 100;
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_from_int_to_decimal(num1, &a);
  s21_from_int_to_decimal(num2, &b);
  s21_decimal res_dec = {0};
  int res_int = 0;
  s21_mul(a, b, &res_dec);
  s21_from_decimal_to_int(res_dec, &res_int);
  ck_assert_int_eq(res_int, prod_int);
}
END_TEST

START_TEST(mul_test_2) {
  int num1 = 10;
  int num2 = 20;
  int prod_int = 200;
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_from_int_to_decimal(num1, &a);
  s21_from_int_to_decimal(num2, &b);
  s21_decimal res_dec = {0};
  int res_int = 0;
  s21_mul(a, b, &res_dec);
  s21_from_decimal_to_int(res_dec, &res_int);
  ck_assert_int_eq(res_int, prod_int);
}
END_TEST

START_TEST(mul_test_4) {
  int num1 = 9403;
  int num2 = 202;
  int res_origin = 1899406;
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_from_int_to_decimal(num1, &a);
  s21_from_int_to_decimal(num2, &b);
  s21_decimal res_dec = {0};
  int res_int = 0;
  s21_mul(a, b, &res_dec);
  s21_from_decimal_to_int(res_dec, &res_int);
  ck_assert_int_eq(res_int, res_origin);
}
END_TEST

START_TEST(mul_test_7) {
  float num1 = 9403.0e2;
  int num2 = 202;
  float res_origin = 189940600;
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_from_float_to_decimal(num1, &a);
  s21_from_int_to_decimal(num2, &b);
  s21_decimal res_dec = {0};
  float res_float = 0;
  s21_mul(a, b, &res_dec);
  s21_from_decimal_to_float(res_dec, &res_float);
  ck_assert_float_eq(res_float, res_origin);
}
END_TEST

START_TEST(mul_test_8) {
  float num1 = 9403.0e2;
  float num2 = 9403.0e2;
  float res_origin = 884164090000;
  s21_decimal a = {0};
  s21_decimal b = {0};
  s21_from_float_to_decimal(num1, &a);
  s21_from_float_to_decimal(num2, &b);
  s21_decimal res_dec = {0};
  float res_float = 0;
  s21_mul(a, b, &res_dec);
  s21_from_decimal_to_float(res_dec, &res_float);
  ck_assert_float_eq(res_float, res_origin);
}
END_TEST

// Value was either too large or too small for a Decimal.
START_TEST(s21_mul_test_9) {
  s21_decimal a = {{-1345409089, 146074521, 0, 0}};  // 627385293423423423
  s21_decimal b = {
      {1668895137, 1064696021, 0, -2147483648}};  // -4572834592045224353
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 2);
  ck_assert_int_eq((int)res_bits.bits[0], 0);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_mul_test_10) {
  s21_decimal a = {{5, 0, 0, -2147483648}};  // -5
  s21_decimal b = {{0, 0, 0, 0}};            // 0
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 0);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_mul_test_11) {
  s21_decimal a = {{351661334, 63675, 0, 0}};  // 273482394234134
  s21_decimal b = {{7, 0, 0, 0}};              // 7
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -1833337958);
  ck_assert_int_eq((int)res_bits.bits[1], 445725);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_mul_test_12) {
  s21_decimal a = {{61237812, 0, 0, -2147024896}};  // -6.1237812
  s21_decimal b = {{81230311, 0, 0, 196608}};       // 81230.311
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -183998228);
  ck_assert_int_eq((int)res_bits.bits[1], 1158184);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2146828288);
}
END_TEST

START_TEST(s21_mul_test_13) {
  s21_decimal a = {{17403, 0, 0, 0}};              // 17403
  s21_decimal b = {{4789282, 0, 0, -2147352576}};  // -47892.82
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 1743496022);
  ck_assert_int_eq((int)res_bits.bits[1], 19);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147352576);
}
END_TEST

START_TEST(s21_mul_test_15) {
  s21_decimal a = {{610781734, 168435, 0, 983040}};  // 0.723423427283494
  s21_decimal b = {{784, 0, 0, 196608}};             // 0.784
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 2111509600);
  ck_assert_int_eq((int)res_bits.bits[1], 132053151);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 1179648);
}
END_TEST

START_TEST(s21_mul_test_16) {
  s21_decimal a = {{7938402, 0, 0, 0}};               // 7938402
  s21_decimal b = {{1830490002, 0, 0, -2147155968}};  // -18304.90002
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -1654667292);
  ck_assert_int_eq((int)res_bits.bits[1], 3383300);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147155968);
}
END_TEST

START_TEST(s21_mul_test_20) {
  s21_decimal a = {{-1318475665, 151177, 0, 0}};          // 649303247399023
  s21_decimal b = {{652206196, 150968, 0, -2147221504}};  // -64840327494.8724
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 446185121);
  ck_assert_int_eq((int)res_bits.bits[1], -1509086043);
  ck_assert_int_eq((int)res_bits.bits[2], -2012665605);
  ck_assert_int_eq((int)res_bits.bits[3], -2147287040);
}
END_TEST

START_TEST(s21_mul_test_21) {
  s21_decimal a = {{0, 0, 0, 0}};  // 0
  s21_decimal b = {{0, 0, 0, 0}};  // 0
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 0);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_mul_test_22) {
  s21_decimal a = {{292342, 0, 0, -2147155968}};  // -2.92342
  s21_decimal b = {{781413, 0, 0, -2147483648}};  // -781413
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 806572558);
  ck_assert_int_eq((int)res_bits.bits[1], 53);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 327680);
}
END_TEST

START_TEST(s21_mul_test_23) {
  s21_decimal a = {{999999999, 0, 0, 0}};  // 999999999
  s21_decimal b = {{999999999, 0, 0, 0}};  // 999999999
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 808348673);
  ck_assert_int_eq((int)res_bits.bits[1], 232830643);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

// Value was either too large or too small for a Decimal.
START_TEST(s21_mul_test_24) {
  s21_decimal a = {{-1, -1, -78293423, 0}};  // 77783903795975396563441680383
  s21_decimal b = {{-2147483648, -2147483648, 628902378,
                    0}};  // 11601201223536716331618402304
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 1);
  ck_assert_int_eq((int)res_bits.bits[0], 0);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

// Value was either too large or too small for a Decimal.
START_TEST(s21_mul_test_25) {
  s21_decimal a = {
      {-1, -1, -78293423, -2147483648}};  // -77783903795975396563441680383
  s21_decimal b = {{-2147483648, -2147483648, 628902378,
                    -2147483648}};  // -11601201223536716331618402304
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 1);
  ck_assert_int_eq((int)res_bits.bits[0], 0);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_mul_test_26) {
  s21_decimal a = {{1, 0, 0, -2147483648}};  // -1
  s21_decimal b = {
      {-1, -1, -1, -2147483648}};  // -79228162514264337593543950335
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -1);
  ck_assert_int_eq((int)res_bits.bits[1], -1);
  ck_assert_int_eq((int)res_bits.bits[2], -1);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_mul_test_27) {
  s21_decimal a = {{8, 0, 0, 0}};        // 8
  s21_decimal b = {{1, 0, 0, 1835008}};  // 0.0000000000000000000000000001
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 8);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 1835008);
}
END_TEST

START_TEST(s21_mul_test_28) {
  s21_decimal a = {{10, 0, 0, 0}};       // 10
  s21_decimal b = {{1, 0, 0, 1835008}};  // 0.0000000000000000000000000001
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 10);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 1835008);
}
END_TEST

START_TEST(s21_mul_test_29) {
  s21_decimal a = {{8, 0, 0, 0}};            // 8
  s21_decimal b = {{1, 0, 0, -2147483648}};  // -1
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 8);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_mul_test_30) {
  s21_decimal a = {{10, 0, 0, 0}};           // 10
  s21_decimal b = {{1, 0, 0, -2147483648}};  // -1
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 10);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_mul_test_31) {
  s21_decimal a = {{1, 0, 0, -2147483648}};  // -1
  s21_decimal b = {{-1, -1, -1, 0}};         // 79228162514264337593543950335
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -1);
  ck_assert_int_eq((int)res_bits.bits[1], -1);
  ck_assert_int_eq((int)res_bits.bits[2], -1);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_mul_test_32) {
  s21_decimal a = {{1, 0, 0, 0}};  // 1
  s21_decimal b = {
      {-1, -1, -1, -2147483648}};  // -79228162514264337593543950335
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -1);
  ck_assert_int_eq((int)res_bits.bits[1], -1);
  ck_assert_int_eq((int)res_bits.bits[2], -1);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_mul_test_37) {
  s21_decimal a = {{-1, -1, -1, 0}};    // 79228162514264337593543950335
  s21_decimal b = {{1, 0, 0, 196608}};  // 0.001
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_mul(a, b, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -1);
  ck_assert_int_eq((int)res_bits.bits[1], -1);
  ck_assert_int_eq((int)res_bits.bits[2], -1);
  ck_assert_int_eq((int)res_bits.bits[3], 196608);
}
END_TEST

//--------------------------s21_div---------------------//

START_TEST(div_0) {
  s21_decimal val1 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{2, 0, 0, 0}};
  s21_decimal res = {{0}};
  ck_assert_int_eq(0, s21_div(val1, val2, &res));
}
END_TEST

START_TEST(div_1) {
  s21_decimal val1 = {{2, 0, 0, 0}};
  s21_decimal val2 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal res;
  ck_assert_int_eq(0, s21_div(val1, val2, &res));
}
END_TEST

START_TEST(div_2) {
  s21_decimal val1 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal res = {{0}};
  ck_assert_int_eq(0, s21_div(val1, val2, &res));
}
END_TEST

START_TEST(div_3) {
  s21_decimal val1 = {{2, 0, 0, ~(UINT_MAX / 2)}};
  s21_decimal val2 = {{0, 0, 0, 0}};
  s21_decimal res = {{0}};
  ck_assert_int_eq(3, s21_div(val1, val2, &res));
}
END_TEST

START_TEST(div_4) {
  s21_decimal val1 = {{2, 0, 0, 0}};
  s21_decimal val2 = {{0, 0, 0, 0}};
  s21_decimal res = {{0}};
  ck_assert_int_eq(3, s21_div(val1, val2, &res));
}
END_TEST

START_TEST(div_5) {
  s21_decimal value_1 = {{35, 0, 0, 0}};
  s21_decimal value_2 = {{5, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal check = {{70, 0, 0, 0}};
  s21_set_power_of_decimal(&value_1, 1);
  s21_set_power_of_decimal(&value_2, 2);
  int return_value = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(div_6) {
  s21_decimal value_1 = {{0x88888888, 0x88888888, 0x88888888, 0}};
  s21_decimal value_2 = {{0x2, 0, 0, 0}};
  s21_set_bit_1(&value_2, 127);
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal check = {{0x44444444, 0x44444444, 0x44444444, 0}};
  s21_set_bit_1(&check, 127);
  int return_value = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(div_7) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  s21_set_bit_1(&value_1, 127);
  s21_decimal value_2 = {{8, 0, 0, 0}};
  s21_set_bit_1(&value_2, 127);
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal check = {{125, 0, 0, 0}};
  s21_set_power_of_decimal(&check, 2);
  int return_value = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(div_8) {
  s21_decimal value_1 = {{15, 0, 0, 0}};
  s21_set_bit_1(&value_1, 127);
  s21_decimal value_2 = {{0, 0, 0, 0}};
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal check = {{0, 0, 0, 0}};
  int return_value = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 3);
}
END_TEST

START_TEST(div_9) {
  s21_decimal value_1 = {{10, 0, 0, 0}};
  s21_decimal value_2 = {{1, 0, 0, 0}};
  s21_set_power_of_decimal(&value_2, 2);
  s21_decimal result = {{0, 0, 0, 0}};
  s21_decimal check = {{1000u, 0, 0, 0}};
  int return_value = s21_div(value_1, value_2, &result);
  ck_assert_int_eq(s21_is_equal(result, check), 1);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

//----------------------s21_negate---------------------//

START_TEST(test_negate1) {
  s21_decimal one = {{0}};
  s21_decimal res = {{0}};
  int ret = s21_negate(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0x80000000);
}
END_TEST

START_TEST(test_negate2) {
  s21_decimal one = {{0, 0, 0, 0x80000000}};
  s21_decimal res = {{0}};
  int ret = s21_negate(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_negate3) {
  s21_decimal one = {{0, 0, 0, 0x80000000}};
  s21_decimal res = {{0}};
  int ret = s21_negate(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_negate4) {
  s21_decimal one = {{0, 0, 0, 0x80000000}};
  s21_decimal res = {{0}};
  int ret = s21_negate(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_negate5) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 0x88000000}};
  s21_decimal res = {{0}};
  int ret = s21_negate(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0x8000000);
}
END_TEST

//--------------------s21_truncate--------------------------//

START_TEST(test_s21_truncate1) {
  s21_decimal one = {{123456789, 0, 0, 0}};
  s21_decimal res = {{0}};
  int ret = s21_truncate(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 123456789);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_s21_truncate2) {
  s21_decimal one = {{123456789, 0, 0, 0}};
  s21_set_power_of_decimal(&one, 1);
  s21_decimal res = {{0}};
  int ret = s21_truncate(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 12345678);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_s21_truncate3) {
  s21_decimal one = {{123456789, 0, 0, 0}};
  s21_set_power_of_decimal(&one, 7);
  s21_decimal res = {{0}};
  int ret = s21_truncate(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 12);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_s21_truncate4) {
  s21_decimal one = {{123456789, 0, 0, 0}};
  s21_set_power_of_decimal(&one, 18);
  s21_decimal res = {{0}};
  int ret = s21_truncate(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(truncate1) {
  s21_decimal x = {{0, 0, 0, last_bits(0, 0)}};
  s21_decimal y = {{0, 0, 0, last_bits(0, 0)}};
  s21_decimal res;
  int i = s21_truncate(x, &res);
  ck_assert_int_eq(i, 0);
  for (int j = 0; j < 4; j++) {
    ck_assert_int_eq(res.bits[j], y.bits[j]);
  }
}
END_TEST

START_TEST(truncate2) {
  s21_decimal x = {{1, 0, 0, last_bits(1, 0)}};
  s21_decimal y = {{0, 0, 0, last_bits(0, 0)}};
  s21_decimal res;
  int i = s21_truncate(x, &res);
  ck_assert_int_eq(i, 0);
  for (int j = 0; j < 4; j++) {
    ck_assert_int_eq(res.bits[j], y.bits[j]);
  }
}
END_TEST

START_TEST(truncate3) {
  s21_decimal x = {{4294967295, 4294967295, 4294967295, last_bits(28, 0)}};
  s21_decimal y = {{7, 0, 0, last_bits(0, 0)}};
  s21_decimal res;
  int i = s21_truncate(x, &res);
  ck_assert_int_eq(i, 0);
  for (int j = 0; j < 4; j++) {
    ck_assert_int_eq(res.bits[j], y.bits[j]);
  }
}
END_TEST

START_TEST(truncate4) {
  s21_decimal x = {{4294967295, 4294967295, 4294967295, last_bits(27, 0)}};
  s21_decimal y = {{79, 0, 0, last_bits(0, 0)}};
  s21_decimal res;
  int i = s21_truncate(x, &res);
  ck_assert_int_eq(i, 0);
  for (int j = 0; j < 4; j++) {
    ck_assert_int_eq(res.bits[j], y.bits[j]);
  }
}
END_TEST

START_TEST(truncate5) {
  s21_decimal x = {{4294967295, 4294967295, 4294967295, last_bits(28, 1)}};
  s21_decimal y = {{7, 0, 0, last_bits(0, 1)}};
  s21_decimal res;
  int i = s21_truncate(x, &res);
  ck_assert_int_eq(i, 0);
  for (int j = 0; j < 4; j++) {
    ck_assert_int_eq(res.bits[j], y.bits[j]);
  }
}
END_TEST

START_TEST(truncate6) {
  s21_decimal x = {{75483, 0, 0, last_bits(5, 1)}};
  s21_decimal y = {{0, 0, 0, last_bits(0, 0)}};
  s21_decimal res;
  int i = s21_truncate(x, &res);
  ck_assert_int_eq(i, 0);
  for (int j = 0; j < 4; j++) {
    ck_assert_int_eq(res.bits[j], y.bits[j]);
  }
}
END_TEST

START_TEST(truncate7) {
  s21_decimal x = {{100000, 0, 0, last_bits(5, 1)}};
  s21_decimal y = {{1, 0, 0, last_bits(0, 1)}};
  s21_decimal res;
  int i = s21_truncate(x, &res);
  ck_assert_int_eq(i, 0);
  for (int j = 0; j < 4; j++) {
    ck_assert_int_eq(res.bits[j], y.bits[j]);
  }
}
END_TEST

//------------------------s21_floor--------------------------//

START_TEST(test_s21_floor1) {
  s21_decimal one = {{75483, 0, 0, 0x80000000}};
  s21_set_power_of_decimal(&one, 5);
  s21_decimal res = {{0}};
  int ret = s21_floor(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 1);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0x80000000);
}
END_TEST

START_TEST(test_s21_floor2) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 0x80000000}};
  s21_set_power_of_decimal(&one, 28);
  s21_decimal res = {{0}};
  int ret = s21_floor(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 8);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0x80000000);
}
END_TEST

START_TEST(test_s21_floor3) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 0}};
  s21_set_power_of_decimal(&one, 28);
  s21_decimal res = {{0}};
  int ret = s21_floor(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 7);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_s21_floor4) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 0}};
  s21_set_power_of_decimal(&one, 1);
  s21_decimal res = {{0}};
  int ret = s21_floor(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 2576980377);
  ck_assert_int_eq(res.bits[1], 2576980377);
  ck_assert_int_eq(res.bits[2], 429496729);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_s21_floor5) {
  s21_decimal one = {{14, 0, 0, 0x80000000}};
  s21_set_power_of_decimal(&one, 1);
  s21_decimal res = {{0}};
  int ret = s21_floor(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 2);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0x80000000);
}
END_TEST

START_TEST(floor6) {
  s21_decimal x = {{75, 0, 0, last_bits(2, 1)}};
  s21_decimal y = {{1, 0, 0, last_bits(0, 1)}};
  s21_decimal res;
  int i = s21_floor(x, &res);
  ck_assert_int_eq(i, 0);
  for (int j = 0; j < 4; j++) {
    ck_assert_int_eq(res.bits[j], y.bits[j]);
  }
}
END_TEST

START_TEST(s21_floor_test_1) {
  s21_decimal a = {{0, 0, 0, 0}};  // 0
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_floor(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 0);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_floor_test_2) {
  s21_decimal a = {{885678421, 180172, 0, 589824}};  // 773833.733333333
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_floor(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 773833);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_floor_test_3) {
  s21_decimal a = {{1317904309, 19, 0, 393216}};  // 82922.282933
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_floor(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 82922);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_floor_test_4) {
  s21_decimal a = {
      {1513097376, 132222, 0, -2146893824}};  //   -567890.678909088
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_floor(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 567891);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_floor_test_5) {
  s21_decimal a = {{-967725593, 870, 0, -2147024896}};  // -373994.8789223
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_floor(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 373995);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_floor_test_7) {
  s21_decimal a = {{-1, -1, -1, 0}};  // 79228162514264337593543950335
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_floor(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -1);
  ck_assert_int_eq((int)res_bits.bits[1], -1);
  ck_assert_int_eq((int)res_bits.bits[2], -1);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_floor_test_8) {
  s21_decimal a = {{1779055156, 182266, 0, 0}};  // 782828288227892
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_floor(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 1779055156);
  ck_assert_int_eq((int)res_bits.bits[1], 182266);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_floor_test_10) {
  s21_decimal a = {{1779615721, 182266, 0, 196608}};  // 782828288788.457
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_floor(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 1144240916);
  ck_assert_int_eq((int)res_bits.bits[1], 182);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_floor_test_11) {
  s21_decimal a = {
      {1779083721, 182266, 0, -2147287040}};  //   -782828288256.457
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_floor(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 1144240385);
  ck_assert_int_eq((int)res_bits.bits[1], 182);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_floor_test_12) {
  s21_decimal a = {{-940046727, 121876, 0, 917504}};  // 5.23456789087865
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_floor(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 5);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_floor_test_13) {
  s21_decimal a = {
      {-1867261908, 148600, 0, -2146697216}};  //   -638.234567890988
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_floor(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 639);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_floor_test_14) {
  s21_decimal a = {{-458206728, 204849, 0, 458752}};  // 87982359.2378872
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_floor(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 87982359);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_floor_test_15) {
  s21_decimal a = {{-2018871682, 41499, 0, -2147352576}};  //-1782391239123.18
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_floor(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -20188716);
  ck_assert_int_eq((int)res_bits.bits[1], 414);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_floor_test_16) {
  s21_decimal a = {{-27362242, 75629, 0, -2147287040}};  // -324828349234.238
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_floor(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -1589165261);
  ck_assert_int_eq((int)res_bits.bits[1], 75);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_floor_test_17) {
  s21_decimal a = {{2116047141, 54697, 0, -2147090432}};  //   -234923942.236453
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_floor(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 234923943);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_floor_test_18) {
  s21_decimal a = {{1832389311, 65997, 0, 720896}};  // 2834.56789023423
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_floor(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 2834);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_floor_test_19) {
  s21_decimal a = {{3, 0, 0, 0}};  // 3
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_floor(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 3);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_floor_test_20) {
  s21_decimal a = {{3, 0, 0, -2147483648}};  // -3
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_floor(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 3);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST
//-----------------------------s21_raund--------------------------//

START_TEST(test_s21_raund1) {
  s21_decimal one = {{75, 0, 0, 0x80000000}};
  s21_set_power_of_decimal(&one, 1);
  s21_decimal res = {{0}};
  int ret = s21_round(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 8);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0x80000000);
}
END_TEST

START_TEST(test_s21_raund2) {
  s21_decimal one = {{0, 0, 0, 0x80000000}};
  s21_set_power_of_decimal(&one, 1);
  s21_decimal res = {{0}};
  int ret = s21_round(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 0);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_s21_raund3) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 0x80000000}};
  s21_set_power_of_decimal(&one, 28);
  s21_decimal res = {{0}};
  int ret = s21_round(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 8);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0x80000000);
}
END_TEST

START_TEST(test_s21_raund4) {
  s21_decimal one = {{165, 0, 0, 0}};
  s21_set_power_of_decimal(&one, 1);
  s21_decimal res = {{0}};
  int ret = s21_round(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 17);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_s21_raund5) {
  s21_decimal one = {{124392478, 0, 0, 0}};
  s21_set_power_of_decimal(&one, 1);
  s21_decimal res = {{0}};
  int ret = s21_round(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 12439248);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_s21_raund6) {
  s21_decimal one = {{124392344, 0, 0, 0x80000000}};
  s21_set_power_of_decimal(&one, 1);
  s21_decimal res = {{0}};
  int ret = s21_round(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 12439234);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0x80000000);
}
END_TEST

START_TEST(test_s21_raund7) {
  s21_decimal one = {{7464923, 0, 0, 0}};
  s21_set_power_of_decimal(&one, 5);
  s21_decimal res = {{0}};
  int ret = s21_round(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 75);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST

START_TEST(test_s21_raund8) {
  s21_decimal one = {{7444923, 0, 0, 0}};
  s21_set_power_of_decimal(&one, 5);

  s21_decimal res = {{0}};
  int ret = s21_round(one, &res);
  ck_assert_int_eq(ret, 0);
  ck_assert_int_eq(res.bits[0], 74);
  ck_assert_int_eq(res.bits[1], 0);
  ck_assert_int_eq(res.bits[2], 0);
  ck_assert_int_eq(res.bits[3], 0);
}
END_TEST
START_TEST(s21_round_test_1) {
  s21_decimal a = {{0, 0, 0, 0}};  // 0
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_round(a, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 0);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_round_test_2) {
  s21_decimal a = {{885678421, 180172, 0, 589824}};  // 773833.733333333
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_round(a, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 773834);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_round_test_4) {
  s21_decimal a = {
      {1513097376, 132222, 0, -2146893824}};  //   -567890.678909088
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_round(a, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 567891);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_round_test_5) {
  s21_decimal a = {{-967725593, 870, 0, -2147024896}};  // -373994.8789223
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_round(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 373995);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_round_test_10) {
  s21_decimal a = {{1779615721, 182266, 0, 196608}};  // 782828288788.457
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_round(a, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 1144240916);
  ck_assert_int_eq((int)res_bits.bits[1], 182);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_round_test_11) {
  s21_decimal a = {
      {1779083721, 182266, 0, -2147287040}};  //   -782828288256.457
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_round(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 1144240384);
  ck_assert_int_eq((int)res_bits.bits[1], 182);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_round_test_12) {
  s21_decimal a = {{-940046727, 121876, 0, 917504}};  // 5.23456789087865
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_round(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 5);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_round_test_13) {
  s21_decimal a = {
      {-1867261908, 148600, 0, -2146697216}};  //  -638.234567890988
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_round(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 638);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_round_test_14) {
  s21_decimal a = {{-458206728, 204849, 0, 458752}};  // 87982359.2378872
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_round(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 87982359);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_round_test_15) {
  s21_decimal a = {{-2018871682, 41499, 0, -2147352576}};  //-1782391239123.18
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_round(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -20188717);
  ck_assert_int_eq((int)res_bits.bits[1], 414);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_round_test_16) {
  s21_decimal a = {{-27362242, 75629, 0, -2147287040}};  // -324828349234.238
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_round(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], -1589165262);
  ck_assert_int_eq((int)res_bits.bits[1], 75);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_round_test_17) {
  s21_decimal a = {{2116047141, 54697, 0, -2147090432}};  //   -234923942.236453
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_round(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 234923942);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_round_test_18) {
  s21_decimal a = {{1832389311, 65997, 0, 720896}};  // 2834.56789023423
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_round(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 2835);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_round_test_19) {
  s21_decimal a = {{3, 0, 0, 0}};  // 3
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_round(a, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 3);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

//--------------------s21_is_equal--------------------------//

START_TEST(test_s21_is_equal1) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 0x88000000}};
  s21_decimal two = {{4294967295, 4294967295, 4294967295, 0x88000000}};
  int ret = s21_is_equal(one, two);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_s21_is_equal2) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 0x88000000}};
  s21_decimal two = {{42949672, 4294967295, 4294967295, 0x88000000}};
  int ret = s21_is_equal(one, two);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(test_s21_is_equal4) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 0)}};
  ck_assert_int_eq(1, s21_is_equal(x, y));
}
END_TEST

START_TEST(test_s21_is_equal5) {
  s21_decimal x = {{123451967, 202, 7, last_bits(6, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 1)}};
  ck_assert_int_eq(0, s21_is_equal(x, y));
}
END_TEST

START_TEST(test_s21_is_equal6) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 1)}};
  s21_decimal y = {{123451967, 202, 2, last_bits(5, 1)}};
  ck_assert_int_eq(0, s21_is_equal(x, y));
}
END_TEST

START_TEST(test_s21_is_equal7) {
  s21_decimal x = {{123451967, 0, 7, last_bits(5, 1)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 1)}};
  ck_assert_int_eq(0, s21_is_equal(x, y));
}
END_TEST

START_TEST(test_s21_is_equal8) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 0)}};
  ck_assert_int_eq(1, s21_is_equal(x, y));
}
END_TEST

START_TEST(test_s21_is_equal9) {
  s21_decimal x = {{4294967295, 4294967295, 4294967295, last_bits(5, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 0)}};
  ck_assert_int_eq(0, s21_is_equal(x, y));
}
END_TEST

START_TEST(test_s21_is_equal10) {
  s21_decimal x = {{4294967295, 4294967295, 4294967295, last_bits(7, 0)}};
  s21_decimal y = {{4294967295, 4294967295, 4294967295, last_bits(7, 0)}};
  ck_assert_int_eq(1, s21_is_equal(x, y));
}
END_TEST

START_TEST(test_s21_is_equal11) {
  s21_decimal x = {{4294967295, 4294967295, 4294967295, last_bits(10, 1)}};
  s21_decimal y = {{4294967295, 4294967295, 4294967295, last_bits(10, 0)}};
  ck_assert_int_eq(0, s21_is_equal(x, y));
}
END_TEST

START_TEST(test_s21_is_equal12) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 0)}};
  s21_decimal y = {{4294967295, 4294967295, 4294967295, last_bits(10, 0)}};
  ck_assert_int_eq(0, s21_is_equal(x, y));
}
END_TEST

START_TEST(s21_round_test_3) {
  s21_decimal a = {{1317904309, 19, 0, 393216}};  // 82922.282933
  s21_decimal res_bits = {{0, 0, 0, 0}};

  int res = s21_round(a, &res_bits);

  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 82922);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

//--------------------s21_is_not_equal--------------------------//

START_TEST(test_s21_is_not_equal_1) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 0x88000000}};
  s21_decimal two = {{42949672, 4294967295, 4294967295, 0x88000000}};
  int ret = s21_is_not_equal(one, two);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_s21_is_not_equal_2) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 0x88000000}};
  s21_decimal two = {{4294967295, 4294967295, 4294967295, 0x88000000}};
  int ret = s21_is_not_equal(one, two);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(test_s21_is_not_equal_3) {
  s21_decimal x = {{123451967, 202, 7, 0}};
  s21_set_power_of_decimal(&x, 5);
  s21_decimal y = {{123451967, 202, 7, 0}};
  s21_set_power_of_decimal(&y, 5);
  ck_assert_int_eq(0, s21_is_not_equal(x, y));
}
END_TEST

START_TEST(test_s21_is_not_equal_4) {
  s21_decimal x = {{123451967, 202, 7, last_bits(6, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 1)}};
  ck_assert_int_eq(1, s21_is_not_equal(x, y));
}
END_TEST

START_TEST(test_s21_is_not_equal_5) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 1)}};
  s21_decimal y = {{123451967, 202, 2, last_bits(5, 1)}};
  ck_assert_int_eq(1, s21_is_not_equal(x, y));
}
END_TEST

START_TEST(test_s21_is_not_equal_6) {
  s21_decimal x = {{123451967, 0, 7, last_bits(5, 1)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 1)}};
  ck_assert_int_eq(1, s21_is_not_equal(x, y));
}
END_TEST

START_TEST(test_s21_is_not_equal_7) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 0)}};
  ck_assert_int_eq(0, s21_is_not_equal(x, y));
}
END_TEST

START_TEST(test_s21_is_not_equal_8) {
  s21_decimal x = {{4294967295, 4294967295, 4294967295, last_bits(5, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 0)}};
  ck_assert_int_eq(1, s21_is_not_equal(x, y));
}
END_TEST

START_TEST(test_s21_is_not_equal_9) {
  s21_decimal x = {{4294967295, 4294967295, 4294967295, last_bits(7, 0)}};
  s21_decimal y = {{4294967295, 4294967295, 4294967295, last_bits(7, 0)}};
  ck_assert_int_eq(0, s21_is_not_equal(x, y));
}
END_TEST

START_TEST(test_s21_is_not_equal_10) {
  s21_decimal x = {{4294967295, 4294967295, 4294967295, last_bits(10, 1)}};
  s21_decimal y = {{4294967295, 4294967295, 4294967295, last_bits(10, 0)}};
  ck_assert_int_eq(1, s21_is_not_equal(x, y));
}
END_TEST

START_TEST(test_s21_is_not_equal_11) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 0)}};
  s21_decimal y = {{4294967295, 4294967295, 4294967295, last_bits(10, 0)}};
  ck_assert_int_eq(1, s21_is_not_equal(x, y));
}
END_TEST

//-----------------------s21_is_greater_or_equal-------------------//

START_TEST(test_s21_is_greater_or_equal1) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 4294967295}};
  s21_decimal two = {{4294967295, 4294967295, 4294967295, 4294967295}};
  int ret = s21_is_greater_or_equal(one, two);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal2) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 0}};
  s21_decimal two = {{4294967295, 4294967295, 4294967295, 1u << 31}};
  int ret = s21_is_greater_or_equal(one, two);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal3) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 1u << 31}};
  s21_decimal two = {{4294967295, 4294967295, 4294967295, 0}};
  int ret = s21_is_greater_or_equal(one, two);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal4) {
  s21_decimal one = {{4294967295, 4294967295, 0, 0}};
  s21_decimal two = {{4294967295, 4294967295, 4294967295, 0}};
  int ret = s21_is_greater_or_equal(one, two);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(test_s21_is_greater_or_equal5) {
  s21_decimal one = {{8, 0, 0, 0}};
  s21_decimal two = {{1, 0, 0, 0}};
  int ret = s21_is_greater_or_equal(one, two);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(is_greater_or_equal_1) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 0)}};
  ck_assert_int_eq(1, s21_is_greater_or_equal(x, y));
}
END_TEST

START_TEST(is_greater_or_equal_2) {
  s21_decimal x = {{123451967, 202, 7, last_bits(6, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 1)}};
  ck_assert_int_eq(1, s21_is_greater_or_equal(x, y));
}
END_TEST

START_TEST(is_greater_or_equal_3) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 1)}};
  s21_decimal y = {{123451967, 202, 2, last_bits(5, 1)}};

  ck_assert_int_eq(0, s21_is_greater_or_equal(x, y));
}
END_TEST

START_TEST(is_greater_or_equal_4) {
  s21_decimal x = {{123451967, 0, 7, last_bits(5, 1)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 1)}};
  ck_assert_int_eq(1, s21_is_greater_or_equal(x, y));
}
END_TEST

START_TEST(is_greater_or_equal_5) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 0)}};
  ck_assert_int_eq(1, s21_is_greater_or_equal(x, y));
}
END_TEST

START_TEST(is_greater_or_equal_6) {
  s21_decimal x = {{4294967295, 4294967295, 4294967295, last_bits(5, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 0)}};
  ck_assert_int_eq(1, s21_is_greater_or_equal(x, y));
}
END_TEST

START_TEST(is_greater_or_equal_7) {
  s21_decimal x = {{4294967295, 4294967295, 4294967295, last_bits(7, 0)}};
  s21_decimal y = {{4294967295, 4294967295, 4294967295, last_bits(7, 0)}};
  ck_assert_int_eq(1, s21_is_greater_or_equal(x, y));
}
END_TEST

START_TEST(is_greater_or_equal_8) {
  s21_decimal x = {{4294967295, 4294967295, 4294967295, last_bits(10, 1)}};
  s21_decimal y = {{4294967295, 4294967295, 4294967295, last_bits(10, 0)}};
  ck_assert_int_eq(0, s21_is_greater_or_equal(x, y));
}
END_TEST

START_TEST(is_greater_or_equal_9) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 0)}};
  s21_decimal y = {{4294967295, 4294967295, 4294967295, last_bits(5, 0)}};
  ck_assert_int_eq(0, s21_is_greater_or_equal(x, y));
}
END_TEST

//-----------------------s21_is_greater-------------------//

START_TEST(test_s21_is_greater1) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 0}};
  s21_decimal two = {{4294967295, 4294967295, 4294967295, 0}};
  int ret = s21_is_greater(one, two);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(test_s21_is_greater2) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 0}};
  s21_decimal two = {{4294967295, 4294967295, 4294967295, 1u << 31}};
  int ret = s21_is_greater(one, two);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_s21_is_greater3) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 1u << 31}};
  s21_decimal two = {{4294967295, 4294967295, 4294967295, 0}};
  int ret = s21_is_greater(one, two);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(test_s21_is_greater4) {
  s21_decimal one = {{4294967295, 4294967295, 0, 0}};
  s21_decimal two = {{4294967295, 4294967295, 4294967295, 0}};
  int ret = s21_is_greater(one, two);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(test_s21_is_greater5) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 0}};
  s21_decimal two = {{4294967295, 4294967295, 4294967294, 0}};
  int ret = s21_is_greater(one, two);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_s21_is_greater6) {
  s21_decimal one = {{300, 0, 0, 0}};
  s21_decimal two = {{200, 0, 0, 0}};
  int ret = s21_is_greater(one, two);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_s21_is_greater7) {
  s21_decimal one = {{8, 0, 0, 0}};
  s21_decimal two = {{1, 0, 0, 0}};
  int ret = s21_is_greater(one, two);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(is_greater_1) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 0)}};
  ck_assert_int_eq(0, s21_is_greater(x, y));
}
END_TEST

START_TEST(is_greater_2) {
  s21_decimal x = {{123451967, 202, 7, last_bits(6, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 1)}};
  ck_assert_int_eq(1, s21_is_greater(x, y));
}
END_TEST

START_TEST(is_greater_3) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 1)}};
  s21_decimal y = {{123451967, 202, 2, last_bits(5, 1)}};
  ck_assert_int_eq(0, s21_is_greater(x, y));
}
END_TEST

START_TEST(is_greater_4) {
  s21_decimal x = {{123451967, 0, 7, last_bits(5, 1)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 1)}};
  ck_assert_int_eq(1, s21_is_greater(x, y));
}
END_TEST

START_TEST(is_greater_5) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 0)}};
  ck_assert_int_eq(0, s21_is_greater(x, y));
}
END_TEST

START_TEST(is_greater_6) {
  s21_decimal x = {{4294967295, 4294967295, 4294967295, last_bits(5, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 0)}};
  ck_assert_int_eq(1, s21_is_greater(x, y));
}
END_TEST

START_TEST(is_greater_7) {
  s21_decimal x = {{4294967295, 4294967295, 4294967295, last_bits(7, 0)}};
  s21_decimal y = {{4294967295, 4294967295, 4294967295, last_bits(7, 0)}};
  ck_assert_int_eq(0, s21_is_greater(x, y));
}
END_TEST

START_TEST(is_greater_8) {
  s21_decimal x = {{4294967295, 4294967295, 4294967295, last_bits(10, 1)}};
  s21_decimal y = {{4294967295, 4294967295, 4294967295, last_bits(10, 0)}};
  ck_assert_int_eq(0, s21_is_greater(x, y));
}
END_TEST

START_TEST(is_greater_9) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 0)}};
  s21_decimal y = {{4294967295, 4294967295, 4294967295, last_bits(5, 0)}};
  ck_assert_int_eq(0, s21_is_greater(x, y));
}
END_TEST

START_TEST(is_greater_10) {
  s21_decimal x = {{123451967, 0, 0, last_bits(5, 1)}};
  s21_decimal y = {{123451968, 0, 0, last_bits(5, 1)}};
  ck_assert_int_eq(1, s21_is_greater(x, y));
}
END_TEST

START_TEST(is_greater_11) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(6, 0)}};
  ck_assert_int_eq(1, s21_is_greater(x, y));
}
END_TEST

START_TEST(is_greater_12) {
  s21_decimal x = {{123451967, 202, 7, last_bits(6, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 0)}};
  ck_assert_int_eq(1, s21_is_greater(y, x));
}
END_TEST

START_TEST(is_greater_13) {
  s21_decimal x = {{17403, 0, 0, 0}};                  // 17403
  s21_decimal y = {{4789282, 0, 0, last_bits(2, 0)}};  // 47892.82
  ck_assert_int_eq(1, great_bez_znak(y, x));
}

START_TEST(is_greater_14) {
  s21_decimal x = {{4294967295, 0, 0}};  // 4294967295
  s21_decimal y = {{1124417, 0, 0}};     // 1124417
  ck_assert_int_eq(0, great_bez_znak(y, x));
}
END_TEST

START_TEST(is_greater_15) {
  s21_decimal x = {{17403, 0, 0, 0}};                  // 4294967295
  s21_decimal y = {{4789282, 0, 0, last_bits(2, 0)}};  // 1124417
  ck_assert_int_eq(0, s21_is_greater(x, y));
}
END_TEST

START_TEST(is_greater_16) {
  s21_decimal x = {{123456, 0, 0, last_bits(1, 0)}};  // 4294967295
  s21_decimal y = {{123456, 0, 0, last_bits(3, 0)}};  // 1124417
  ck_assert_int_eq(1, s21_is_greater(x, y));
}
END_TEST

START_TEST(is_greater_17) {
  s21_decimal x = {{12345656, 0, 0, last_bits(3, 0)}};   // 4294967295
  s21_decimal y = {{123456789, 0, 0, last_bits(8, 0)}};  // 1124417
  ck_assert_int_eq(1, s21_is_greater(x, y));
}
END_TEST

//-----------------------s21_is_less_or_equal-------------------//

START_TEST(test_s21_is_less_or_equal1) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 4294967295}};
  s21_decimal two = {{4294967295, 4294967295, 4294967295, 4294967295}};
  int ret = s21_is_less_or_equal(one, two);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_s21_is_less_or_equal2) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 0}};
  s21_decimal two = {{4294967295, 4294967295, 4294967295, 1u << 31}};
  int ret = s21_is_less_or_equal(one, two);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(test_s21_is_less_or_equal3) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 1u << 31}};
  s21_decimal two = {{4294967295, 4294967295, 4294967295, 0}};
  int ret = s21_is_less_or_equal(one, two);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_s21_is_less_or_equal4) {
  s21_decimal one = {{4294967295, 4294967295, 0, 0}};
  s21_decimal two = {{4294967295, 4294967295, 4294967295, 0}};
  int ret = s21_is_less_or_equal(one, two);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(is_less_or_equal_1) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 0)}};
  ck_assert_int_eq(1, s21_is_less_or_equal(x, y));
}
END_TEST

START_TEST(is_less_or_equal_2) {
  s21_decimal x = {{123451967, 202, 7, last_bits(6, 1)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 0)}};
  ck_assert_int_eq(1, s21_is_less_or_equal(x, y));
}
END_TEST

START_TEST(is_less_or_equal_3) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 1)}};
  s21_decimal y = {{123451967, 202, 2, last_bits(5, 1)}};
  ck_assert_int_eq(1, s21_is_less_or_equal(x, y));
}
END_TEST

START_TEST(is_less_or_equal_4) {
  s21_decimal x = {{123451967, 0, 7, last_bits(5, 1)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 1)}};
  ck_assert_int_eq(0, s21_is_less_or_equal(x, y));
}
END_TEST

START_TEST(is_less_or_equal_5) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 0)}};
  ck_assert_int_eq(1, s21_is_less_or_equal(x, y));
}
END_TEST

START_TEST(is_less_or_equal_6) {
  s21_decimal x = {{4294967295, 4294967295, 4294967295, last_bits(5, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 0)}};
  ck_assert_int_eq(0, s21_is_less_or_equal(x, y));
}
END_TEST

START_TEST(is_less_or_equal_7) {
  s21_decimal x = {{4294967295, 4294967295, 4294967295, last_bits(7, 0)}};
  s21_decimal y = {{4294967295, 4294967295, 4294967295, last_bits(7, 0)}};
  ck_assert_int_eq(1, s21_is_less_or_equal(x, y));
}
END_TEST

START_TEST(is_less_or_equal_8) {
  s21_decimal x = {{4294967295, 4294967295, 4294967295, last_bits(10, 1)}};
  s21_decimal y = {{4294967295, 4294967295, 4294967295, last_bits(10, 0)}};
  ck_assert_int_eq(1, s21_is_less_or_equal(x, y));
}
END_TEST

START_TEST(is_less_or_equal_9) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 0)}};
  s21_decimal y = {{4294967295, 4294967295, 4294967295, last_bits(10, 0)}};
  ck_assert_int_eq(1, s21_is_less_or_equal(x, y));
}
END_TEST

//-----------------------s21_is_less-------------------//

START_TEST(test_s21_is_less1) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 0}};
  s21_decimal two = {{4294967295, 4294967295, 4294967295, 0}};
  int ret = s21_is_less(one, two);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(test_s21_is_less2) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 0}};
  s21_decimal two = {{4294967295, 4294967295, 4294967295, 1u << 31}};
  int ret = s21_is_less(one, two);
  ck_assert_int_eq(ret, 0);
}
END_TEST

START_TEST(test_s21_is_less3) {
  s21_decimal one = {{4294967295, 4294967295, 4294967295, 1u << 31}};
  s21_decimal two = {{4294967295, 4294967295, 4294967295, 0}};
  int ret = s21_is_less(one, two);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_s21_is_less4) {
  s21_decimal one = {{4294967295, 4294967295, 0, 0}};
  s21_decimal two = {{4294967295, 4294967295, 4294967295, 0}};
  int ret = s21_is_less(one, two);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(test_s21_is_less5) {
  s21_decimal one = {{200, 0, 0, 0}};
  s21_decimal two = {{300, 0, 0, 0}};
  int ret = s21_is_less(one, two);
  ck_assert_int_eq(ret, 1);
}
END_TEST

START_TEST(is_less_1) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 0)}};
  ck_assert_int_eq(0, s21_is_less(x, y));
}
END_TEST

START_TEST(is_less_2) {
  s21_decimal x = {{123451967, 202, 7, last_bits(6, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 0)}};
  ck_assert_int_eq(1, s21_is_less(x, y));
}
END_TEST

START_TEST(is_less_3) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 0)}};
  s21_decimal y = {{123451967, 202, 2, last_bits(5, 0)}};
  ck_assert_int_eq(0, s21_is_less(x, y));
}
END_TEST

START_TEST(is_less_4) {
  s21_decimal x = {{123451967, 0, 7, last_bits(5, 1)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 1)}};
  ck_assert_int_eq(0, s21_is_less(x, y));
}
END_TEST

START_TEST(is_less_5) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 1)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 0)}};
  ck_assert_int_eq(1, s21_is_less(x, y));
}
END_TEST

START_TEST(is_less_6) {
  s21_decimal x = {{4294967295, 4294967295, 4294967295, last_bits(5, 0)}};
  s21_decimal y = {{123451967, 202, 7, last_bits(5, 0)}};
  ck_assert_int_eq(0, s21_is_less(x, y));
}
END_TEST

START_TEST(is_less_7) {
  s21_decimal x = {{4294967295, 4294967295, 4294967295, last_bits(7, 0)}};
  s21_decimal y = {{4294967295, 4294967295, 4294967295, last_bits(7, 0)}};
  ck_assert_int_eq(0, s21_is_less(x, y));
}
END_TEST

START_TEST(is_less_8) {
  s21_decimal x = {{4294967295, 4294967295, 4294967295, last_bits(10, 1)}};
  s21_decimal y = {{4294967295, 4294967295, 4294967295, last_bits(10, 0)}};
  ck_assert_int_eq(1, s21_is_less(x, y));
}
END_TEST

START_TEST(is_less_9) {
  s21_decimal x = {{123451967, 202, 7, last_bits(5, 0)}};
  s21_decimal y = {{4294967295, 4294967295, 4294967295, last_bits(10, 0)}};
  ck_assert_int_eq(1, s21_is_less(x, y));
}
END_TEST

// --------------------------------s21_set_exp-----------------//

START_TEST(test_s21_set_exp1) {
  s21_decimal one = {{0, 0, 0, 0x80000000}};
  s21_set_power_of_decimal(&one, 1);
  ck_assert_int_eq(one.bits[0], 0);
  ck_assert_int_eq(one.bits[1], 0);
  ck_assert_int_eq(one.bits[2], 0);
  ck_assert_int_eq(one.bits[3], 2147549184);
}
END_TEST

START_TEST(test_s21_set_exp2) {
  s21_decimal one = {{0, 0, 0, 0x80000000}};
  s21_set_power_of_decimal(&one, 25);
  ck_assert_int_eq(one.bits[0], 0);
  ck_assert_int_eq(one.bits[1], 0);
  ck_assert_int_eq(one.bits[2], 0);
  ck_assert_int_eq(one.bits[3], 2149122048);
}
// END_TEST

//--------------------------------s21_vbltashitb_exp-----------------//

// START_TEST(test_s21_vbltashitb_exp) {
//   s21_decimal one = {{123456789, 0, 0, last_bits(8, 0)}};
//   s21_decimal res = {{0}};
//   s21_vbltashitb_exp(one, &res);
//   ck_assert_int_eq(res.bits[0], 23456789);
//   ck_assert_int_eq(res.bits[1], 0);
//   ck_assert_int_eq(res.bits[2], 0);
//   ck_assert_int_eq(res.bits[3], 0);
// }
// END_TEST

// START_TEST(test_s21_vbltashitb_exp2) {
//   s21_decimal one = {{1234567811, 0, 0, last_bits(9, 1)}};
//   s21_decimal res = {{0}};
//   s21_vbltashitb_exp(one, &res);
//   ck_assert_int_eq(res.bits[0], 234567811);
//   ck_assert_int_eq(res.bits[1], 0);
//   ck_assert_int_eq(res.bits[2], 0);
//   ck_assert_int_eq(res.bits[3], 0);
// }
// END_TEST

//------------------decimal to float-------------------//
START_TEST(from_decimal_to_float_0) {
  s21_decimal val = {{123456789, 0, 0, 0}};
  s21_set_power_of_decimal(&val, 5);
  float res = 1234.56789;
  float tmp = 0.;
  float *dst = &tmp;
  s21_from_decimal_to_float(val, dst);
  ck_assert_float_eq(*dst, res);
}
END_TEST

START_TEST(from_decimal_to_float_1) {
  s21_decimal val = {{123456789, 0, 0, 0}};
  s21_set_bit(&val, 127, 1);
  s21_set_power_of_decimal(&val, 5);
  float res = -1234.56789;
  float tmp = 0.;
  float *dst = &tmp;
  s21_from_decimal_to_float(val, dst);
  ck_assert_float_eq(*dst, res);
}
END_TEST

START_TEST(from_decimal_to_float_2) {
  s21_decimal var = {{0, 0, 0, 0}};
  float value = 0;
  s21_from_float_to_decimal(5.0, &var);
  s21_from_decimal_to_float(var, &value);
  ck_assert_float_eq_tol(5.0, value, 10e-5);
}
END_TEST

START_TEST(from_decimal_to_float_3) {
  float n = s21_rand_r(-8388608, 8388608);
  s21_decimal var = {{0, 0, 0, 0}};
  float value = 0;
  s21_from_float_to_decimal(n, &var);
  s21_from_decimal_to_float(var, &value);
  ck_assert_float_eq_tol(n, value, 10);
}
END_TEST

START_TEST(from_decimal_to_float_4) {
  float n = s21_rand_r(-1e-6 / 3, 1e-6 / 3);
  s21_decimal var = {{0, 0, 0, 0}};
  float value = 0;
  s21_from_float_to_decimal(n, &var);
  s21_from_decimal_to_float(var, &value);
  ck_assert_float_eq_tol(n, value, 10);
}
END_TEST

START_TEST(s21_from_decimal_to_float_1) {
  s21_decimal var = {0};
  float value = 0;
  s21_from_float_to_decimal(5.0, &var);
  s21_from_decimal_to_float(var, &value);
  ck_assert_float_eq_tol(5.0, value, 10e-5);
}
END_TEST

START_TEST(s21_from_decimal_to_float_2) {
  float n = s21_rand_r(-8388608, 8388608);
  s21_decimal var = {0};
  float value = 0;
  s21_from_float_to_decimal(n, &var);
  s21_from_decimal_to_float(var, &value);
  ck_assert_float_eq_tol(n, value, 10);
}
END_TEST

START_TEST(s21_from_decimal_to_float_3) {
  float n = s21_rand_r(-1e-6 / 3, 1e-6 / 3);
  s21_decimal var = {0};
  float value = 0;
  s21_from_float_to_decimal(n, &var);
  s21_from_decimal_to_float(var, &value);
  ck_assert_float_eq_tol(n, value, 10);
}
END_TEST

START_TEST(s21_from_decimal_to_float_4) {
  s21_decimal value = {{1812, 0, 0, 0}};
  s21_set_bit_1(&value, 127);
  float result = 0;
  float check = -1812;
  int return_value = s21_from_decimal_to_float(value, &result);
  ck_assert_float_eq(result, check);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_5) {
  s21_decimal value = {{18122, 0, 0, 0}};
  s21_set_power_of_decimal(&value, 3);
  s21_set_bit_1(&value, 127);
  float result = 0;
  float check = -18.122;
  int return_value = s21_from_decimal_to_float(value, &result);
  ck_assert_float_eq(result, check);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_6) {
  s21_decimal value = {{0xFFFFFF, 0, 0, 0}};
  float result = 0;
  float check = 16777215;
  int return_value = s21_from_decimal_to_float(value, &result);
  ck_assert_float_eq(result, check);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_7) {
  s21_decimal value = {{0xFFFFFFFF, 0xFFFFFFFF, 0, 0}};
  float result = 0;
  float check = 0xFFFFFFFFFFFFFFFF;
  int return_value = s21_from_decimal_to_float(value, &result);
  ck_assert_float_eq(result, check);
  ck_assert_int_eq(return_value, 0);
}
END_TEST

START_TEST(s21_from_decimal_to_float_8) {
  s21_decimal src = {0};
  int result = 0;
  float number = 0.0;
  src.bits[0] = 23450987;
  src.bits[1] = 0;
  src.bits[2] = 0;
  src.bits[3] = 2147745792;
  result = s21_from_decimal_to_float(src, &number);
  ck_assert_float_eq(number, -2345.0987);
  ck_assert_int_eq(result, 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal_test_1) {
  float num = 829834.3;
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_from_float_to_decimal(num, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 8298343);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 65536);
}
END_TEST

START_TEST(s21_from_float_to_decimal_test_2) {
  float num = 8934.278;
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_from_float_to_decimal(num, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 8934278);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 196608);
}
END_TEST

START_TEST(s21_from_float_to_decimal_test_3) {
  float num = -23423.82;
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_from_float_to_decimal(num, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 2342382);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147352576);
}
END_TEST

START_TEST(s21_from_float_to_decimal_test_5) {
  float num = 823923;
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_from_float_to_decimal(num, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 823923);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal_test_7) {
  float num = 23744.78;
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_from_float_to_decimal(num, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 2374478);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 131072);
}
END_TEST

START_TEST(s21_from_float_to_decimal_test_8) {
  float num = -2393.823;
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_from_float_to_decimal(num, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 2393823);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147287040);
}
END_TEST

START_TEST(s21_from_float_to_decimal_test_9) {
  float num = 912479.2;
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_from_float_to_decimal(num, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 9124792);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 65536);
}
END_TEST

START_TEST(s21_from_float_to_decimal_test_10) {
  float num = -123234.7;
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_from_float_to_decimal(num, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 1232347);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147418112);
}
END_TEST

START_TEST(s21_from_float_to_decimal_test_11) {
  float num = 10121.7;
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_from_float_to_decimal(num, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 101217);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 65536);
}
END_TEST

START_TEST(s21_from_float_to_decimal_test_12) {
  float num = -23748280;
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_from_float_to_decimal(num, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 23748280);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147483648);
}
END_TEST

START_TEST(s21_from_float_to_decimal_test_13) {
  float num = 802000;
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_from_float_to_decimal(num, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 802000);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 0);
}
END_TEST

START_TEST(s21_from_float_to_decimal_test_14) {
  float num = 1.3331;
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_from_float_to_decimal(num, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 13331);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 262144);
}
END_TEST

START_TEST(s21_from_float_to_decimal_test_16) {
  float num = 0.5632482;
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_from_float_to_decimal(num, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 5632482);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], 458752);
}
END_TEST

START_TEST(s21_from_float_to_decimal_test_17) {
  float num = -0.6527385;
  s21_decimal res_bits = {{0, 0, 0, 0}};
  int res = s21_from_float_to_decimal(num, &res_bits);
  ck_assert_int_eq((int)res, 0);
  ck_assert_int_eq((int)res_bits.bits[0], 6527385);
  ck_assert_int_eq((int)res_bits.bits[1], 0);
  ck_assert_int_eq((int)res_bits.bits[2], 0);
  ck_assert_int_eq((int)res_bits.bits[3], -2147024896);
}
END_TEST

Suite *s21_decimal_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("s21_decimal");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_s21_from_int_to_decimal_1);
  tcase_add_test(tc_core, test_s21_from_int_to_decimal_2);
  tcase_add_test(tc_core, test_s21_from_int_to_decimal_3);
  tcase_add_test(tc_core, from_int_to_decimal);
  tcase_add_test(tc_core, from_int_to_decimal2);
  tcase_add_test(tc_core, from_int_to_decimal3);

  tcase_add_test(tc_core, s21_from_decimal_to_int_1);
  tcase_add_test(tc_core, s21_from_decimal_to_int_2);
  tcase_add_test(tc_core, s21_from_decimal_to_int_3);
  tcase_add_test(tc_core, s21_from_decimal_to_int_4);
  tcase_add_test(tc_core, s21_from_decimal_to_int_5);
  tcase_add_test(tc_core, s21_from_decimal_to_int_6);
  tcase_add_test(tc_core, s21_from_decimal_to_int_7);
  tcase_add_test(tc_core, from_decimal_to_int);
  tcase_add_test(tc_core, from_decimal_to_int2);

  tcase_add_test(tc_core, test_add);
  tcase_add_test(tc_core, test_add2);
  tcase_add_test(tc_core, test_add3);
  tcase_add_test(tc_core, test_add4);
  tcase_add_test(tc_core, test_add5);
  tcase_add_test(tc_core, test_add6);
  tcase_add_test(tc_core, test_add7);
  tcase_add_test(tc_core, test_add8);
  tcase_add_test(tc_core, add1);
  tcase_add_test(tc_core, add2);
  tcase_add_test(tc_core, add3);
  tcase_add_test(tc_core, add4);
  tcase_add_test(tc_core, add5);
  tcase_add_test(tc_core, add6);
  tcase_add_test(tc_core, add7);
  tcase_add_test(tc_core, add8);
  tcase_add_test(tc_core, add9);
  tcase_add_test(tc_core, add12);
  tcase_add_test(tc_core, add13);
  tcase_add_test(tc_core, add14);
  tcase_add_test(tc_core, add15);
  tcase_add_test(tc_core, add16);
  tcase_add_test(tc_core, add17);
  tcase_add_test(tc_core, s21_add_test_1);
  tcase_add_test(tc_core, s21_add_test_2);
  tcase_add_test(tc_core, s21_add_test_3);
  tcase_add_test(tc_core, s21_add_test_4);
  tcase_add_test(tc_core, s21_add_test_5);
  tcase_add_test(tc_core, s21_add_test_6);
  tcase_add_test(tc_core, s21_add_test_7);
  tcase_add_test(tc_core, s21_add_test_8);
  tcase_add_test(tc_core, s21_add_test_9);
  tcase_add_test(tc_core, s21_add_test_10);
  tcase_add_test(tc_core, s21_add_test_11);
  tcase_add_test(tc_core, s21_add_test_12);
  tcase_add_test(tc_core, s21_add_test_13);
  tcase_add_test(tc_core, s21_add_test_14);
  tcase_add_test(tc_core, s21_add_test_15);
  tcase_add_test(tc_core, s21_add_test_16);
  tcase_add_test(tc_core, s21_add_test_17);
  tcase_add_test(tc_core, s21_add_test_18);
  tcase_add_test(tc_core, s21_add_test_19);
  tcase_add_test(tc_core, s21_add_test_20);
  tcase_add_test(tc_core, s21_add_test_21);
  tcase_add_test(tc_core, s21_add_test_22);
  tcase_add_test(tc_core, s21_add_test_23);
  tcase_add_test(tc_core, s21_add_test_24);
  tcase_add_test(tc_core, s21_add_test_25);
  tcase_add_test(tc_core, s21_add_test_26);
  tcase_add_test(tc_core, s21_add_test_29);
  tcase_add_test(tc_core, s21_add_test_30);
  tcase_add_test(tc_core, s21_add_test_31);
  tcase_add_test(tc_core, s21_add_test_32);
  tcase_add_test(tc_core, s21_add_test_33);
  tcase_add_test(tc_core, s21_add_test_34);
  tcase_add_test(tc_core, s21_add_test_35);
  tcase_add_test(tc_core, s21_add_test_36);
  tcase_add_test(tc_core, s21_add_test_37);

  tcase_add_test(tc_core, test_sub1);
  tcase_add_test(tc_core, test_sub2);
  tcase_add_test(tc_core, test_sub3);
  tcase_add_test(tc_core, test_sub4);
  tcase_add_test(tc_core, test_sub5);
  tcase_add_test(tc_core, test_sub6);
  tcase_add_test(tc_core, test_sub7);
  tcase_add_test(tc_core, test_sub8);
  tcase_add_test(tc_core, test_sub9);
  tcase_add_test(tc_core, test_sub10);
  tcase_add_test(tc_core, test_sub11);
  tcase_add_test(tc_core, test_sub12);
  tcase_add_test(tc_core, test_sub14);
  tcase_add_test(tc_core, test_sub15);
  tcase_add_test(tc_core, test_sub16);
  tcase_add_test(tc_core, test_sub17);
  tcase_add_test(tc_core, test_sub18);
  tcase_add_test(tc_core, test_sub19);
  tcase_add_test(tc_core, test_sub20);

  tcase_add_test(tc_core, test_mul1);
  tcase_add_test(tc_core, test_mul2);
  tcase_add_test(tc_core, test_mul3);
  tcase_add_test(tc_core, test_mul4);
  tcase_add_test(tc_core, test_mul5);
  tcase_add_test(tc_core, test_mul6);
  tcase_add_test(tc_core, s21_mul_test_1);
  tcase_add_test(tc_core, s21_mul_test_2);
  tcase_add_test(tc_core, s21_mul_test_3);
  tcase_add_test(tc_core, s21_mul_test_4);
  tcase_add_test(tc_core, s21_mul_test_5);
  tcase_add_test(tc_core, s21_mul_test_6);
  tcase_add_test(tc_core, s21_mul_test_7);
  tcase_add_test(tc_core, s21_mul_test_8);
  tcase_add_test(tc_core, s21_mul_test_9);
  tcase_add_test(tc_core, s21_mul_test_10);
  tcase_add_test(tc_core, s21_mul_test_11);
  tcase_add_test(tc_core, s21_mul_test_12);
  tcase_add_test(tc_core, s21_mul_test_13);
  tcase_add_test(tc_core, s21_mul_test_15);
  tcase_add_test(tc_core, s21_mul_test_16);
  tcase_add_test(tc_core, s21_mul_test_20);
  tcase_add_test(tc_core, s21_mul_test_21);
  tcase_add_test(tc_core, s21_mul_test_22);
  tcase_add_test(tc_core, s21_mul_test_23);
  tcase_add_test(tc_core, s21_mul_test_24);
  tcase_add_test(tc_core, s21_mul_test_25);
  tcase_add_test(tc_core, s21_mul_test_26);
  tcase_add_test(tc_core, s21_mul_test_27);
  tcase_add_test(tc_core, s21_mul_test_28);
  tcase_add_test(tc_core, s21_mul_test_29);
  tcase_add_test(tc_core, s21_mul_test_30);
  tcase_add_test(tc_core, s21_mul_test_31);
  tcase_add_test(tc_core, s21_mul_test_32);
  tcase_add_test(tc_core, s21_mul_test_37);
  tcase_add_test(tc_core, mul_0);
  tcase_add_test(tc_core, mul_1);
  tcase_add_test(tc_core, mul_2);
  tcase_add_test(tc_core, mul_3);
  tcase_add_test(tc_core, mul_4);
  tcase_add_test(tc_core, mul_5);
  tcase_add_test(tc_core, mul_6);
  tcase_add_test(tc_core, mul_7);
  tcase_add_test(tc_core, mul_8);
  tcase_add_test(tc_core, mul_9);
  tcase_add_test(tc_core, mul_10);
  tcase_add_test(tc_core, mul_11);
  tcase_add_test(tc_core, mul_12);
  tcase_add_test(tc_core, mul_13);
  tcase_add_test(tc_core, mul_14);
  tcase_add_test(tc_core, mul_15);
  tcase_add_test(tc_core, mul_16);
  tcase_add_test(tc_core, mul_17);
  tcase_add_test(tc_core, mul_18);
  tcase_add_test(tc_core, mul_test_1);
  tcase_add_test(tc_core, mul_test_2);
  tcase_add_test(tc_core, mul_test_4);
  tcase_add_test(tc_core, mul_test_7);
  tcase_add_test(tc_core, mul_test_8);

  tcase_add_test(tc_core, test_negate1);
  tcase_add_test(tc_core, test_negate2);
  tcase_add_test(tc_core, test_negate3);
  tcase_add_test(tc_core, test_negate4);
  tcase_add_test(tc_core, test_negate5);

  tcase_add_test(tc_core, test_s21_truncate1);
  tcase_add_test(tc_core, test_s21_truncate2);
  tcase_add_test(tc_core, test_s21_truncate3);
  tcase_add_test(tc_core, test_s21_truncate4);
  tcase_add_test(tc_core, truncate1);
  tcase_add_test(tc_core, truncate2);
  tcase_add_test(tc_core, truncate3);
  tcase_add_test(tc_core, truncate4);
  tcase_add_test(tc_core, truncate5);
  tcase_add_test(tc_core, truncate6);
  tcase_add_test(tc_core, truncate7);

  tcase_add_test(tc_core, test_s21_floor1);
  tcase_add_test(tc_core, test_s21_floor2);
  tcase_add_test(tc_core, test_s21_floor3);
  tcase_add_test(tc_core, test_s21_floor4);
  tcase_add_test(tc_core, test_s21_floor5);
  tcase_add_test(tc_core, floor6);
  tcase_add_test(tc_core, s21_floor_test_1);
  tcase_add_test(tc_core, s21_floor_test_2);
  tcase_add_test(tc_core, s21_floor_test_3);
  tcase_add_test(tc_core, s21_floor_test_4);
  tcase_add_test(tc_core, s21_floor_test_5);
  tcase_add_test(tc_core, s21_floor_test_7);
  tcase_add_test(tc_core, s21_floor_test_8);
  tcase_add_test(tc_core, s21_floor_test_10);
  tcase_add_test(tc_core, s21_floor_test_11);
  tcase_add_test(tc_core, s21_floor_test_12);
  tcase_add_test(tc_core, s21_floor_test_13);
  tcase_add_test(tc_core, s21_floor_test_14);
  tcase_add_test(tc_core, s21_floor_test_15);
  tcase_add_test(tc_core, s21_floor_test_16);
  tcase_add_test(tc_core, s21_floor_test_17);
  tcase_add_test(tc_core, s21_floor_test_18);
  tcase_add_test(tc_core, s21_floor_test_19);
  tcase_add_test(tc_core, s21_floor_test_20);

  tcase_add_test(tc_core, test_s21_raund1);
  tcase_add_test(tc_core, test_s21_raund2);
  tcase_add_test(tc_core, test_s21_raund3);
  tcase_add_test(tc_core, test_s21_raund4);
  tcase_add_test(tc_core, test_s21_raund5);
  tcase_add_test(tc_core, test_s21_raund6);
  tcase_add_test(tc_core, test_s21_raund7);
  tcase_add_test(tc_core, test_s21_raund8);
  tcase_add_test(tc_core, s21_round_test_1);
  tcase_add_test(tc_core, s21_round_test_2);
  tcase_add_test(tc_core, s21_round_test_3);
  tcase_add_test(tc_core, s21_round_test_4);
  tcase_add_test(tc_core, s21_round_test_5);
  tcase_add_test(tc_core, s21_round_test_10);
  tcase_add_test(tc_core, s21_round_test_11);
  tcase_add_test(tc_core, s21_round_test_12);
  tcase_add_test(tc_core, s21_round_test_13);
  tcase_add_test(tc_core, s21_round_test_14);
  tcase_add_test(tc_core, s21_round_test_15);
  tcase_add_test(tc_core, s21_round_test_16);
  tcase_add_test(tc_core, s21_round_test_17);
  tcase_add_test(tc_core, s21_round_test_18);
  tcase_add_test(tc_core, s21_round_test_19);

  tcase_add_test(tc_core, test_s21_is_equal1);
  tcase_add_test(tc_core, test_s21_is_equal2);
  tcase_add_test(tc_core, test_s21_is_equal5);
  tcase_add_test(tc_core, test_s21_is_equal4);
  tcase_add_test(tc_core, test_s21_is_equal6);
  tcase_add_test(tc_core, test_s21_is_equal7);
  tcase_add_test(tc_core, test_s21_is_equal8);
  tcase_add_test(tc_core, test_s21_is_equal9);
  tcase_add_test(tc_core, test_s21_is_equal10);
  tcase_add_test(tc_core, test_s21_is_equal11);
  tcase_add_test(tc_core, test_s21_is_equal12);

  tcase_add_test(tc_core, test_s21_is_not_equal_1);
  tcase_add_test(tc_core, test_s21_is_not_equal_2);
  tcase_add_test(tc_core, test_s21_is_not_equal_3);
  tcase_add_test(tc_core, test_s21_is_not_equal_4);
  tcase_add_test(tc_core, test_s21_is_not_equal_5);
  tcase_add_test(tc_core, test_s21_is_not_equal_6);
  tcase_add_test(tc_core, test_s21_is_not_equal_7);
  tcase_add_test(tc_core, test_s21_is_not_equal_8);
  tcase_add_test(tc_core, test_s21_is_not_equal_9);
  tcase_add_test(tc_core, test_s21_is_not_equal_10);
  tcase_add_test(tc_core, test_s21_is_not_equal_11);

  tcase_add_test(tc_core, test_s21_is_greater_or_equal1);
  tcase_add_test(tc_core, test_s21_is_greater_or_equal2);
  tcase_add_test(tc_core, test_s21_is_greater_or_equal3);
  tcase_add_test(tc_core, test_s21_is_greater_or_equal4);
  tcase_add_test(tc_core, test_s21_is_greater_or_equal5);
  tcase_add_test(tc_core, is_greater_or_equal_1);
  tcase_add_test(tc_core, is_greater_or_equal_2);
  tcase_add_test(tc_core, is_greater_or_equal_3);
  tcase_add_test(tc_core, is_greater_or_equal_4);
  tcase_add_test(tc_core, is_greater_or_equal_5);
  tcase_add_test(tc_core, is_greater_or_equal_6);
  tcase_add_test(tc_core, is_greater_or_equal_7);
  tcase_add_test(tc_core, is_greater_or_equal_8);
  tcase_add_test(tc_core, is_greater_or_equal_9);

  tcase_add_test(tc_core, test_s21_is_greater1);
  tcase_add_test(tc_core, test_s21_is_greater2);
  tcase_add_test(tc_core, test_s21_is_greater3);
  tcase_add_test(tc_core, test_s21_is_greater4);
  tcase_add_test(tc_core, test_s21_is_greater5);
  tcase_add_test(tc_core, test_s21_is_greater6);
  tcase_add_test(tc_core, test_s21_is_greater7);
  tcase_add_test(tc_core, is_greater_1);
  tcase_add_test(tc_core, is_greater_2);
  tcase_add_test(tc_core, is_greater_3);
  tcase_add_test(tc_core, is_greater_4);
  tcase_add_test(tc_core, is_greater_5);
  tcase_add_test(tc_core, is_greater_6);
  tcase_add_test(tc_core, is_greater_7);
  tcase_add_test(tc_core, is_greater_8);
  tcase_add_test(tc_core, is_greater_9);
  tcase_add_test(tc_core, is_greater_10);
  tcase_add_test(tc_core, is_greater_11);
  tcase_add_test(tc_core, is_greater_12);
  tcase_add_test(tc_core, is_greater_13);
  tcase_add_test(tc_core, is_greater_14);
  tcase_add_test(tc_core, is_greater_15);
  tcase_add_test(tc_core, is_greater_16);
  tcase_add_test(tc_core, is_greater_17);

  tcase_add_test(tc_core, test_s21_is_less_or_equal1);
  tcase_add_test(tc_core, test_s21_is_less_or_equal2);
  tcase_add_test(tc_core, test_s21_is_less_or_equal3);
  tcase_add_test(tc_core, test_s21_is_less_or_equal4);
  tcase_add_test(tc_core, is_less_or_equal_1);
  tcase_add_test(tc_core, is_less_or_equal_2);
  tcase_add_test(tc_core, is_less_or_equal_3);
  tcase_add_test(tc_core, is_less_or_equal_4);
  tcase_add_test(tc_core, is_less_or_equal_5);
  tcase_add_test(tc_core, is_less_or_equal_6);
  tcase_add_test(tc_core, is_less_or_equal_7);
  tcase_add_test(tc_core, is_less_or_equal_8);
  tcase_add_test(tc_core, is_less_or_equal_9);

  tcase_add_test(tc_core, test_s21_is_less1);
  tcase_add_test(tc_core, test_s21_is_less2);
  tcase_add_test(tc_core, test_s21_is_less3);
  tcase_add_test(tc_core, test_s21_is_less4);
  tcase_add_test(tc_core, test_s21_is_less5);
  tcase_add_test(tc_core, is_less_1);
  tcase_add_test(tc_core, is_less_2);
  tcase_add_test(tc_core, is_less_3);
  tcase_add_test(tc_core, is_less_4);
  tcase_add_test(tc_core, is_less_5);
  tcase_add_test(tc_core, is_less_6);
  tcase_add_test(tc_core, is_less_7);
  tcase_add_test(tc_core, is_less_8);
  tcase_add_test(tc_core, is_less_9);

  tcase_add_test(tc_core, test_s21_set_exp1);
  tcase_add_test(tc_core, test_s21_set_exp2);

  // tcase_add_test(tc_core, test_s21_vbltashitb_exp);
  // tcase_add_test(tc_core, test_s21_vbltashitb_exp2);

  tcase_add_test(tc_core, div_0);
  tcase_add_test(tc_core, div_1);
  tcase_add_test(tc_core, div_2);
  tcase_add_test(tc_core, div_3);
  tcase_add_test(tc_core, div_4);
  tcase_add_test(tc_core, div_5);
  tcase_add_test(tc_core, div_6);
  tcase_add_test(tc_core, div_7);
  tcase_add_test(tc_core, div_8);
  tcase_add_test(tc_core, div_9);

  tcase_add_test(tc_core, from_decimal_to_float_0);
  tcase_add_test(tc_core, from_decimal_to_float_1);
  tcase_add_test(tc_core, from_decimal_to_float_2);
  tcase_add_test(tc_core, s21_from_decimal_to_float_1);
  tcase_add_test(tc_core, s21_from_decimal_to_float_4);
  tcase_add_test(tc_core, s21_from_decimal_to_float_5);
  tcase_add_test(tc_core, s21_from_decimal_to_float_6);
  tcase_add_test(tc_core, s21_from_decimal_to_float_7);
  tcase_add_test(tc_core, s21_from_decimal_to_float_8);
  tcase_add_loop_test(tc_core, from_decimal_to_float_3, 0, 100);
  tcase_add_loop_test(tc_core, from_decimal_to_float_4, 0, 100);
  tcase_add_loop_test(tc_core, s21_from_decimal_to_float_2, 0, 100);
  tcase_add_loop_test(tc_core, s21_from_decimal_to_float_3, 0, 100);
  tcase_add_test(tc_core, s21_from_float_to_decimal_test_1);
  tcase_add_test(tc_core, s21_from_float_to_decimal_test_2);
  tcase_add_test(tc_core, s21_from_float_to_decimal_test_3);
  tcase_add_test(tc_core, s21_from_float_to_decimal_test_5);
  tcase_add_test(tc_core, s21_from_float_to_decimal_test_7);
  tcase_add_test(tc_core, s21_from_float_to_decimal_test_8);
  tcase_add_test(tc_core, s21_from_float_to_decimal_test_9);
  tcase_add_test(tc_core, s21_from_float_to_decimal_test_10);
  tcase_add_test(tc_core, s21_from_float_to_decimal_test_11);
  tcase_add_test(tc_core, s21_from_float_to_decimal_test_12);
  tcase_add_test(tc_core, s21_from_float_to_decimal_test_13);
  tcase_add_test(tc_core, s21_from_float_to_decimal_test_14);
  tcase_add_test(tc_core, s21_from_float_to_decimal_test_16);
  tcase_add_test(tc_core, s21_from_float_to_decimal_test_17);

  suite_add_tcase(s, tc_core);

  return s;
}

int main(void) {
  int number_failed;
  Suite *s;
  SRunner *sr;

  s = s21_decimal_suite();
  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
