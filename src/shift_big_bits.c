#include "s21_decimal.h"

/*
  Ну.. тут все понятно
  Сдвиги вправо, используются в умножении

  Каждый сдвиг зависит от вышестоящих интов, если мы собираемся перенести бит из
  нулевого инта в первый, то проверяем первый ряд, если там все ок (31 бит в 1
  инте свободен), то двигаем его, ставим бит в 1 ряд, ставим 0 в 0 ряд и двигаем
  его.

  2-  ...           ...        ...        ...

  1-  0010..00  ->  0100..00 -> 0100..01 -> 0100..01

  0-  1000..01      1000..01    0000..01    0000..10
*/
void twist_bit_big(s21_big_decimal *dec_num, int first, int second) {
  s21_set_bit_1_big(dec_num, second);
  s21_set_bit_0_big(dec_num, first);
}

void shift_big_bit_left(s21_big_decimal *value, int count, int number_shift,
                        int end_shift) {
  for (; number_shift <= end_shift; number_shift++) {
    value->bits[number_shift] <<= count;
  }
}
void shift_bit_right_big(s21_big_decimal *value, int count, int number_shift) {
  for (; number_shift <= 5; number_shift++) {
    value->bits[number_shift] >>= count;
  }
}

int s21_shift_159_big(s21_big_decimal *dec_num) {
  if (s21_get_bit_big(dec_num, 191)) {
    return (2);
  } else {
    dec_num->bits[5] = dec_num->bits[5] << 1;
    twist_bit_big(dec_num, 159, 160);
    dec_num->bits[4] = dec_num->bits[4] << 1;
  }
  return (0);
}

int s21_shift_127_big(s21_big_decimal *dec_num, int *flg_159,
                      int *flg_overlow) {
  if (s21_get_bit_big(dec_num, 159)) {
    *flg_overlow = s21_shift_159_big(dec_num);
    twist_bit_big(dec_num, 127, 128);
    dec_num->bits[3] = dec_num->bits[3] << 1;
    *flg_159 = 0;
  } else {
    dec_num->bits[4] = dec_num->bits[4] << 1;
    twist_bit_big(dec_num, 127, 128);
    dec_num->bits[3] = dec_num->bits[3] << 1;
  }
  return (0);
}

int s21_shift_95_big(s21_big_decimal *dec_num, int *flg_127, int *flg_159,
                     int *flg_overlow) {
  if (s21_get_bit_big(dec_num, 127)) {
    s21_shift_127_big(dec_num, flg_159, flg_overlow);
    twist_bit_big(dec_num, 95, 96);
    dec_num->bits[2] = dec_num->bits[2] << 1;
    *flg_127 = 0;
  } else {
    dec_num->bits[3] = dec_num->bits[3] << 1;
    twist_bit_big(dec_num, 95, 96);
    dec_num->bits[2] = dec_num->bits[2] << 1;
  }
  return (0);
}

int s21_shift_63_big(s21_big_decimal *dec_num, int *flg_95, int *flg_127,
                     int *flg_159, int *flg_overlow) {
  if (s21_get_bit_big(dec_num, 95)) {
    s21_shift_95_big(dec_num, flg_127, flg_159, flg_overlow);
    twist_bit_big(dec_num, 63, 64);
    dec_num->bits[1] = dec_num->bits[1] << 1;
    *flg_95 = 0;
  } else {
    dec_num->bits[2] = dec_num->bits[2] << 1;
    twist_bit_big(dec_num, 63, 64);
    dec_num->bits[1] = dec_num->bits[1] << 1;
  }
  return (0);
}

int s21_shift_31_big(s21_big_decimal *dec_num, int *flg_63, int *flg_95,
                     int *flg_127, int *flg_159, int *flg_overlow) {
  if (s21_get_bit_big(dec_num, 63)) {
    s21_shift_63_big(dec_num, flg_95, flg_127, flg_159, flg_overlow);
    twist_bit_big(dec_num, 31, 32);
    dec_num->bits[0] = dec_num->bits[0] << 1;

    *flg_63 = 0;
  } else {
    dec_num->bits[1] = dec_num->bits[1] << 1;
    twist_bit_big(dec_num, 31, 32);
    dec_num->bits[0] = dec_num->bits[0] << 1;
  }
  return (0);
}

int s21_shift_bits_big(s21_big_decimal *dec_num, int index) {
  int flg_159 = 1;
  int flg_127 = 1;
  int flg_95 = 1;
  int flg_63 = 1;
  int flg_31 = 1;
  int flg_overlow = 1;
  s21_big_decimal tmp = *dec_num;  // вернуть при overflow

  while (index) {  // или index != -1
    if (s21_get_bit_big(dec_num, 191)) {
      return (0);
    }
    if (s21_get_bit_big(dec_num, 31)) {
      flg_31 = s21_shift_31_big(dec_num, &flg_63, &flg_95, &flg_127, &flg_159,
                                &flg_overlow);
    }
    if (s21_get_bit_big(dec_num, 63) && flg_63 && flg_31) {
      flg_63 =
          s21_shift_63_big(dec_num, &flg_95, &flg_127, &flg_159, &flg_overlow);
    }
    if (s21_get_bit_big(dec_num, 95) && flg_95 && flg_63) {
      flg_95 = s21_shift_95_big(dec_num, &flg_127, &flg_159, &flg_overlow);
    }
    if (s21_get_bit_big(dec_num, 127) && flg_127 && flg_95) {
      flg_127 = s21_shift_127_big(dec_num, &flg_159, &flg_overlow);
    }
    if (s21_get_bit_big(dec_num, 159) && flg_159 && flg_127) {
      flg_159 = s21_shift_159_big(dec_num);
    }

    if (flg_overlow == 2) {
      return (0);  // ошибка (overflow)
      *dec_num = tmp;
    } else {
      if (flg_31) {
        if (flg_63)
          shift_big_bit_left(dec_num, 1, 0, 1);
        else
          shift_big_bit_left(dec_num, 1, 0, 0);
        flg_31 = 0;
      }
      if (flg_63) {
        if (flg_31) {
          shift_big_bit_left(dec_num, 1, 1, 2);
        } else {
          if (flg_95) {
            shift_big_bit_left(dec_num, 1, 2, 2);
          }
          flg_63 = 0;
        }
      }
      if (flg_95) {
        if (flg_63) {
          shift_big_bit_left(dec_num, 1, 2, 3);
        } else {
          if (flg_127) {
            shift_big_bit_left(dec_num, 1, 3, 3);
          }
        }
        flg_95 = 0;
      }
      if (flg_127) {
        if (flg_95) {
          shift_big_bit_left(dec_num, 1, 3, 4);
        } else {
          if (flg_159) {
            shift_big_bit_left(dec_num, 1, 4, 4);
          }
          flg_127 = 0;
        }
      }
      if (flg_159) {
        if (flg_127)
          shift_big_bit_left(dec_num, 1, 4, 5);
        else
          shift_big_bit_left(dec_num, 1, 5, 5);
      }
    }
    flg_159 = 1;
    flg_127 = 1;
    flg_95 = 1;
    flg_63 = 1;
    flg_31 = 1;
    index--;
  }

  return (1);  // при ошибке вернет 0
}
