#include "s21_decimal.h"

// следует добавить проверку на переполнение 95 бита

void s21_shift_31(s21_decimal *dec_num, int flg_31, int flg_63) {
  if (flg_63) {
    dec_num->bits[2] = dec_num->bits[2] << 1;
    dec_num->bits[1] = dec_num->bits[1] << 1;
  }
  if (flg_31 && !flg_63) dec_num->bits[1] = dec_num->bits[1] << 1;
  s21_set_bit_1(dec_num, 32);
  s21_set_bit_0(dec_num, 31);
  dec_num->bits[0] = dec_num->bits[0] << 1;
}

void s21_shift_63(s21_decimal *dec_num) {
  dec_num->bits[2] = dec_num->bits[2] << 1;
  s21_set_bit_1(dec_num, 64);
  s21_set_bit_0(dec_num, 63);
  dec_num->bits[1] = dec_num->bits[1] << 1;
}

// осталось только ускорить на 32 - индекс (-)
void s21_shift_bits(s21_decimal *dec_num, int index) {
  int flg_31 = 1;
  int flg_63 = 1;
  while (index) {
    if (s21_get_bit(dec_num, 63)) {
      s21_shift_63(dec_num);
      flg_63 = 0;

      if (s21_get_bit(dec_num, 31)) {
        s21_shift_31(dec_num, 0, flg_63);
        flg_31 = 0;
      }

      if (flg_31) dec_num->bits[0] = dec_num->bits[0] << 1;
      flg_31 = 0;
    }
    if (s21_get_bit(dec_num, 31) && flg_31) {
      s21_shift_31(dec_num, flg_31, flg_63);
      flg_31 = 0;
      flg_63 = 0;
    }
    if (flg_31 && flg_63) {  // додумай
      dec_num->bits[2] = dec_num->bits[2] << 1;
      dec_num->bits[1] = dec_num->bits[1] << 1;
      dec_num->bits[0] = dec_num->bits[0] << 1;
    }
    flg_31 = 1;
    flg_63 = 1;
    index--;
  }
  // dec_num->bits[getRow] = dec_num->bits[getRow] << index;
}