#include "fizzle/fizzle.hpp"
#include <iostream>

#define code "std::int32_t main() { \n"                             \
             "fizzle::tokens tokens;\n"                             \
             "tokens.tokenize(code, fizzle::tokenizer_langs::c);\n" \
             "std::cout << tokens.extract_str() << std::endl;\n"    \
             "return 0;\n"                                          \
             "}"

std::int32_t main() {
      fizzle::tokens tokens;
      tokens.tokenize(code, fizzle::tokenizer_langs::c);
      std::cout << tokens.extract_str() << std::endl;
      return 0;
}