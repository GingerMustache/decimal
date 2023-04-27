#include "s21_decimal.h"
// все возникшие ошибки могут быть из за добавления сдвигов влво и вправо !!
// добавил flg_overflow_mul и все, что с ним связано
void shift_bit_left(s21_decimal *value, int count) {
  value->bits[2] <<= count;
  value->bits[1] <<= count;
  value->bits[0] <<= count;
}
void shift_bit_right(s21_decimal *value, int count) {
  value->bits[2] >>= count;
  value->bits[1] >>= count;
  value->bits[0] >>= count;
}

// следует добавить проверку на переполнение 95 бита
int s21_shift_31(s21_decimal *dec_num, int flg_31, int flg_63) {
  int output = 0;
  if (!s21_get_bit(dec_num, 95)) {  // стоит ли 95 бит
    if (flg_63) {                   // был ли 63 бит
      dec_num->bits[2] = dec_num->bits[2] << 1;
      dec_num->bits[1] = dec_num->bits[1] << 1;
    }
    if (flg_31 && !flg_63) dec_num->bits[1] = dec_num->bits[1] << 1;
    s21_set_bit_1(dec_num, 32);
    s21_set_bit_0(dec_num, 31);
    dec_num->bits[0] = dec_num->bits[0] << 1;
    output = 1;
  }
  return (output);
}

int s21_shift_63(s21_decimal *dec_num) {
  int output = 0;
  if (!s21_get_bit(dec_num, 95)) {
    dec_num->bits[2] = dec_num->bits[2] << 1;  // только если 95 бит свободен
    s21_set_bit_1(dec_num, 64);
    s21_set_bit_0(dec_num, 63);
    dec_num->bits[1] = dec_num->bits[1] << 1;
    output = 1;
  }
  return (output);
}

// добавить проверку для 95 при приклепнии проверки на переполнение
int check_index_shift(s21_decimal dec_num, int index) {
  int output = 0;
  int _63 = 63;
  int _31 = 31;
  int _95 = 95;

  int get_63 = 0;
  int get_31 = 0;
  int get_95 = 0;

  while (_63 > 31 && _31 > -1) {
    if (get_63 != 2) get_63 = s21_get_bit(&dec_num, _63);
    if (get_31 != 2) get_31 = s21_get_bit(&dec_num, _31);
    if (get_95 != 2) get_95 = s21_get_bit(&dec_num, _95);
    // get_95 = s21_get_bit(&dec_num, _95);
    if (get_95 == 1) {
      if (_95 + index > 95) {
        return (0);
      } else {
        output++;
        get_95 = 2;
      }
    }
    if (get_63 == 1) {
      if (_63 + index > 63) {
        return (0);
      } else {
        output++;
        get_63 = 2;
      }
    }
    if (get_31 == 1) {
      if (_31 + index > 31) {
        return (0);
      } else {
        output++;
        get_31 = 2;
      }
    }
    if (output == 3 || (output == 1 && (!_63 || !_31 || !_95))) return (2);
    _95--;
    _63--;
    _31--;
  }
  return (0);
}

int s21_shift_bits(s21_decimal *dec_num, int index) {
  int flg_overlow_mul = 1;
  int flg_63 = 1;
  int flg_31 = 1;
  int flg_index = 1;
  int flg_overlow_index = 1;

  while (index && flg_index && flg_overlow_mul) {
    if (s21_get_bit(dec_num, 63)) {
      flg_overlow_mul = s21_shift_63(dec_num);
      flg_63 = 0;

      if (s21_get_bit(dec_num, 31) && flg_overlow_mul) {
        s21_shift_31(dec_num, 0, flg_63);
        flg_31 = 0;
      }

      if (flg_31 && flg_overlow_mul) dec_num->bits[0] = dec_num->bits[0] << 1;
      flg_31 = 0;
    }
    if (s21_get_bit(dec_num, 31) && flg_31 && flg_overlow_mul) {
      flg_overlow_mul = s21_shift_31(dec_num, flg_31, flg_63);
      flg_31 = 0;
      flg_63 = 0;
    }
    if (flg_31 && flg_63 && flg_overlow_mul) {
      if (check_index_shift(*dec_num, index) && flg_overlow_index) {
        /*
           если сдвиг можно сделать без перехода в другие разряды то двигаем все
           на index сразу
        */
        // dec_num->bits[2] <<= index;
        // dec_num->bits[1] <<= index;
        // dec_num->bits[0] <<= index;
        shift_bit_left(dec_num, index);
        flg_index = 0;
      } else {
        /*
           если нет, но двигаем все на 1 смотрим дальше
        */
        // dec_num->bits[2] <<= 1;
        // dec_num->bits[1] <<= 1;
        // dec_num->bits[0] <<= 1;
        shift_bit_left(dec_num, 1);
        flg_overlow_index = 0;
      }
    }
    flg_31 = 1;
    flg_63 = 1;
    index--;
  }
  return flg_overlow_mul == 1 ? 0 : 1;
}
