#pragma once
#include <cstdint>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

namespace fizzle {

      enum class tokenizer_langs : std::uint8_t {
            none,
            lua,
            lua_kosmic, /* https://github.com/Paxsma/Kosmic/ */
            cpp,
            c,
            x86asm,
            x64asm,
            python,
            javascript,
            java
      };

      class tokens {

          public:
            tokens() = default;
            ~tokens() = default;

            /* Extracts tokens and turn them into a representation string of it. */
            std::string extract_str() const {
                  return std::accumulate(this->raw.begin(), this->raw.end(), std::string(), [](std::string result, const auto &i) {
                        result += "{" + i + "} ";
                        return result;
                  });
            }

            /* Extracts tokens and turn them into a representation json. */
            std::string extract_json() const {
                  std::string result = "[\n";
                  for (const auto &i : this->raw) {
                        result += "\"" + ((i == "\"" || i == "\'") ? "\\" + i : i) + "\"" + ((&i == &this->raw.back()) ? "\n" : ",\n");
                  }
                  result += "]";
                  return result;
            }

            /* Extracts raw tokens. */
            std::vector<std::string> extract() const {
                  return this->raw;
            }

            /* Tokenizes code with given lang, ignores comments. */
            void tokenize(const std::string &code, const tokenizer_langs l = tokenizer_langs::none);

          private: /* ADD SCOPING ID */
            std::vector<std::string /* token */> raw;
            tokenizer_langs lang = tokenizer_langs::none;
            std::string code = "";

            void tok(const std::string &s) {
                  this->raw.emplace_back(s);
            }

            void tok(const char s) {
                  this->raw.emplace_back(std::string(1u, s));
            }

            void parser(const char *const one_line_comment, const char *const mul_line_comment_begin, const char *const mul_line_comment_end);
      };
} // namespace fizzle