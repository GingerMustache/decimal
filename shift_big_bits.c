#include "s21_decimal.h"
// все возникшие ошибки могут быть из за добавления сдвигов влво и вправо !!
// добавил flg_overflow_mul и все, что с ним связано
void twist_bit_big(s21_big_decimal *dec_num, int first, int second) {
  s21_set_bit_1_big(dec_num, first);
  s21_set_bit_0_big(dec_num, second);
}

void shift_big_bit_left(s21_big_decimal *value, int count) {
  value->bits[5] <<= count;
  value->bits[4] <<= count;
  value->bits[3] <<= count;
  value->bits[2] <<= count;
  value->bits[1] <<= count;
  value->bits[0] <<= count;
}
void shift_bit_right_big(s21_big_decimal *value, int count) {
  value->bits[5] >>= count;
  value->bits[4] >>= count;
  value->bits[3] >>= count;
  value->bits[2] >>= count;
  value->bits[1] >>= count;
  value->bits[0] >>= count;
}

// следует добавить проверку на переполнение 95 бита

int s21_shift_159_big(s21_big_decimal *dec_num) {
  dec_num->bits[5] = dec_num->bits[5] << 1;  // двигаем ряд выше
  twist_bit_big(dec_num, 159, 160);
  dec_num->bits[4] = dec_num->bits[4] << 1;  // двигаем нанешний ряд
  return (0);
}

int s21_shift_127_big(s21_big_decimal *dec_num, int flg_159) {
  if (!flg_159) {  // если выше все отлично
    dec_num->bits[5] = dec_num->bits[5] << 1;  // двигаем ряд выше
    dec_num->bits[4] = dec_num->bits[4] << 1;  // двигаем ряд выше
  }
  twist_bit_big(dec_num, 127, 128);
  dec_num->bits[3] = dec_num->bits[3] << 1;  // двигаем нанешний ряд
  return (0);
}

int s21_shift_95_big(s21_big_decimal *dec_num, int flg_127) {
  if (!flg_127) {  // если выше все отлично
    dec_num->bits[5] = dec_num->bits[5] << 1;  // двигаем ряд выше
    dec_num->bits[4] = dec_num->bits[4] << 1;  // двигаем ряд выше
    dec_num->bits[3] = dec_num->bits[3] << 1;  // двигаем ряд выше
  }
  twist_bit_big(dec_num, 95, 96);
  dec_num->bits[2] = dec_num->bits[2] << 1;  // двигаем нанешний ряд
  return (0);
}

int s21_shift_63_big(s21_big_decimal *dec_num, int flg_95) {
  if (!flg_95) {  // если выше все отлично
    dec_num->bits[5] = dec_num->bits[5] << 1;  // двигаем ряд выше
    dec_num->bits[4] = dec_num->bits[4] << 1;  // двигаем ряд выше
    dec_num->bits[3] = dec_num->bits[3] << 1;  // двигаем ряд выше
    dec_num->bits[2] = dec_num->bits[2] << 1;  // двигаем ряд выше
  }
  twist_bit_big(dec_num, 63, 64);
  dec_num->bits[1] = dec_num->bits[1] << 1;  // двигаем нанешний ряд
  return (0);
}

int s21_shift_31_big(s21_big_decimal *dec_num, int flg_63) {
  if (!flg_63) {  // если выше все отлично
    dec_num->bits[5] = dec_num->bits[5] << 1;  // двигаем ряд выше
    dec_num->bits[4] = dec_num->bits[4] << 1;  // двигаем ряд выше
    dec_num->bits[3] = dec_num->bits[3] << 1;  // двигаем ряд выше
    dec_num->bits[2] = dec_num->bits[2] << 1;  // двигаем ряд выше
    dec_num->bits[1] = dec_num->bits[1] << 1;  // двигаем ряд выше
  }
  twist_bit_big(dec_num, 31, 32);
  dec_num->bits[0] = dec_num->bits[0] << 1;  // двигаем нанешний ряд
  return (0);
}

// добавить проверку для 95 при приклепнии проверки на переполнение
int check_index_shift_big(s21_big_decimal dec_num, int index) {
  int output = 0;
  int _31 = 31;
  int _63 = 63;
  int _95 = 95;
  int _127 = 127;
  int _159 = 159;
  int _191 = 191;

  int get_63 = 0;
  int get_31 = 0;
  int get_95 = 0;
  int get_127 = 0;
  int get_159 = 0;
  int get_191 = 0;

  while (_31 > -1) {
    if (get_63 != 2) get_63 = s21_get_bit_big(&dec_num, _63);
    if (get_31 != 2) get_31 = s21_get_bit_big(&dec_num, _31);
    if (get_95 != 2) get_95 = s21_get_bit_big(&dec_num, _95);
    if (get_127 != 2) get_63 = s21_get_bit_big(&dec_num, _63);
    if (get_159 != 2) get_31 = s21_get_bit_big(&dec_num, _31);
    if (get_191 != 2) get_95 = s21_get_bit_big(&dec_num, _95);

    if (index + _31 >= 31) {
      if (get_191 == 1) {
        if (_191 + index > 191) {
          return (0);
        } else {
          output++;
          get_191 = 2;
        }
      }

      if (get_159 == 1) {
        if (_159 + index > 159) {
          return (0);
        } else {
          output++;
          get_159 = 2;
        }
      }

      if (get_127 == 1) {
        if (_127 + index > 127) {
          return (0);
        } else {
          output++;
          get_127 = 2;
        }
      }

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
    }
    if (output > 0 || (output == 1 /*может 2*/ && (!_63 || !_31 || !_95)))
      return (2);
    _191--;
    _159--;
    _127--;
    _95--;
    _63--;
    _31--;
  }
  return (0);
}

int s21_shift_bits_big(s21_big_decimal *dec_num, int index) {
  int flg_191 = 1;
  int flg_159 = 1;
  int flg_127 = 1;
  int flg_95 = 1;
  int flg_63 = 1;
  int flg_31 = 1;
  int flg_index = 1;
  int flg_overlow_index = 1;

  while (index && flg_index) {
    if (s21_get_bit_big(dec_num, 191)) {  // 5
      return (0);                         // overflow
    }
    if (s21_get_bit_big(dec_num, 159)) {  // 4
      flg_159 = s21_shift_159_big(dec_num);
    }
    if (s21_get_bit_big(dec_num, 127)) {  // 3
      flg_127 = s21_shift_127_big(dec_num, flg_159);
    }
    if (s21_get_bit_big(dec_num, 95)) {  // 2
      flg_95 = s21_shift_95_big(dec_num, flg_127);
    }
    if (s21_get_bit_big(dec_num, 63)) {  // 1
      flg_63 = s21_shift_63_big(dec_num, flg_95);
    }
    if (s21_get_bit_big(dec_num, 31)) {  // 0
      flg_63 = s21_shift_31_big(dec_num, flg_63);
    }

    // ускорение, но надо ли оно?
    if (flg_31 && flg_63) {
      if (check_index_shift_big(*dec_num, index) && flg_overlow_index) {
        /*
           если сдвиг можно сделать без перехода в другие разряды то двигаем
           все на index сразу
        */
        shift_big_bit_left(dec_num, index);
        flg_index = 0;
      } else {
        /*
           если нет, но двигаем все на 1 смотрим дальше
        */
        shift_big_bit_left(dec_num, 1);
        flg_overlow_index = 0;
      }
    }
    flg_191 = 1;
    flg_159 = 1;
    flg_95 = 1;
    flg_63 = 1;
    flg_31 = 1;
    index--;
  }
  return (1);  // при ошибке вернет 0
}
