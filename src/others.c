#include "s21_decimal.h"

// Убираем дробную часть
int s21_truncate(s21_decimal value, s21_decimal *result) {
  int output = CONVERSATION_ERROR;

  if (result) {
    output = CONVERSATION_OK;
    int power = s21_get_power_of_decimal(value);
    s21_set_power_of_decimal(&value, 0);
    while (power) {
      s21_div_decimal_by_10(&value);
      power--;
    }
    *result = value;
  }
  return output;
}

// decimal -= decimal
int s21_negate(s21_decimal value, s21_decimal *result) {
  int output = CONVERSATION_ERROR;
  if (result) {
    if (s21_get_bit(&value, 127)) {
      s21_set_bit_0(&value, 127);
    } else {
      s21_set_bit_1(&value, 127);
    }
    *result = value;
    output = CONVERSATION_OK;
  }
  return output;
}

// Округление в меньшую сторону
int s21_floor(s21_decimal value, s21_decimal *result) {
  int res = 0;
  int power_of_decimal = s21_get_power_of_decimal(value);
  s21_decimal mask = {{1, 0, 0, 0}};

  if (power_of_decimal != 0) {
    s21_cut_exp(&value, power_of_decimal);
    s21_set_power_of_decimal(&value, 0);
    *result = value;

    if (s21_get_bits(value.bits[3], 31)) {
      s21_set_bit_0(&value, 127);
      s21_add(value, mask, result);
      s21_set_bit_1(result, 127);
    }

  } else
    *result = value;

  return res;
}

// Округление в большую сторону
int s21_round(s21_decimal value, s21_decimal *result) {
  int output = CONVERSATION_OK;

  if (!result) {
    // если указатель на decimal является NULL
    output = CONVERSATION_ERROR;
  } else {
    // В остальных случаях округляем
    s21_set_dec_number_to_0(result);
    int sign = s21_get_bit(&value, 127);
    s21_decimal fractional;
    s21_decimal value_unsigned_truncated;
    // Убираем знак
    s21_decimal value_unsigned = value;
    s21_set_bit_0(&value_unsigned, 127);
    // Убираем дробную часть числа
    s21_truncate(value_unsigned, &value_unsigned_truncated);

    // Считаем убранную дробную часть числа
    s21_sub(value_unsigned, value_unsigned_truncated, &fractional);

    // Производим округление, исходя из дробной части числа
    value_unsigned_truncated =
        s21_round_banking(value_unsigned_truncated, fractional);

    *result = value_unsigned_truncated;
    // Возвращаем знак
    if (sign) {  // постановка знака
      s21_set_bit_1(result, 127);
    } else {
      s21_set_bit_0(result, 127);
    }
  }

  return output;
}

// Банковское округление
s21_decimal s21_round_banking(s21_decimal integral, s21_decimal fractional) {
  s21_decimal zerofive = s21_decimal_get_zerofive();
  s21_decimal result;
  s21_decimal decimal_one = {{1, 0, 0, 0}};

  if (s21_is_equal(fractional, zerofive)) {
    // Если дробная часть ровно 0.5
    if (s21_decimal_even(integral)) {
      // Если целая часть четная, то оставляем её
      result = integral;
    } else {
      // Если целая часть нечетная, то увеличиваем её на 1
      s21_add(integral, decimal_one, &result);
    }
  } else if (s21_is_greater(fractional, zerofive)) {
    // Если дробная часть > 0.5, то увеличиваем целую часть на 1
    s21_add(integral, decimal_one, &result);
  } else {
    // Если дробная часть < 0.5, то оставляем целую часть без изменений
    result = integral;
  }

  return result;
}
