#include <Arduino.h>
#include "utils.h"

// Transform input string in kebab-case
String toKebabCase(String input) {
  String result = "";
  for (int i = 0; i < input.length(); i++) {
    char c = input[i];
    if (c == ' ' || c == '_' || c == '-') {
      if (result.length() > 0 && result[result.length() - 1] != '-')
        result += '-';
    } else {
      result += (char)tolower(c);
    }
  }
  if (result.endsWith("-")) result.remove(result.length() - 1);
  return result;
}