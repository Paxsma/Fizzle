#include "fizzle.hpp"
#include <iostream>

void fizzle::tokens::parser(const char *const one_line_comment, const char *const mul_line_comment_begin, const char *const mul_line_comment_end) {
      std::string token = "";
      bool escape = false; /* Ignores char with threshold. */
      bool in_string = false;
      char prev = '\0';

      /* Remove comments */
      {
            size_t pos = 0u;
            bool in_string = false;
            while (pos < this->code.length()) {
                  if (this->code[pos] == '"' || this->code[pos] == '\'') {
                        in_string = !in_string;
                        ++pos;
                  }
                  if (!in_string && !this->code.compare(pos, std::strlen(one_line_comment), one_line_comment)) {
                        const auto end = this->code.find('\n', pos);
                        if (end == std::string::npos) {
                              this->code.erase(pos);
                              continue;
                        }
                        const auto mul_begin = this->code.find(mul_line_comment_begin, pos);
                        if (mul_begin != std::string::npos && mul_begin < end) {
                              const auto mul_end_pos = this->code.find(mul_line_comment_end, mul_begin);
                              if (mul_end_pos != std::string::npos) {
                                    this->code.erase(pos, mul_end_pos - pos + std::strlen(mul_line_comment_end));
                                    continue;
                              }
                        }
                        this->code.erase(pos, end - pos);
                  } else if (!in_string && !this->code.compare(pos, strlen(mul_line_comment_begin), mul_line_comment_begin)) {
                        const auto end = this->code.find(mul_line_comment_end, pos);
                        if (end == std::string::npos) {
                              this->code.erase(pos);
                              continue;
                        }
                        this->code.erase(pos, end - pos + std::strlen(mul_line_comment_end));
                  } else {
                        ++pos;
                  }
            }
      }

      for (auto it = this->code.begin(); it != this->code.end(); ++it) {
            const auto i = *it;
            const auto next = (it + 1u != code.end()) ? *(it + 1u) : '\0';
            if (!escape) {
                  switch (i) {
                        case '(':
                        case ')':
                        case '[':
                        case ']':
                        case '{':
                        case '}':
                        case '=':
                        case '+':
                        case '^':
                        case '*':
                        case '%':
                        case ';':
                        case ':':
                        case ',':
                        case '|':
                        case '#':
                        case '@':
                        case '&':
                        case '-':
                        case '.': {
                              if (i == '.' && std::isdigit(prev)) {
                                    token += i;
                                    continue;
                              }
                              if (!in_string) {
                                    if (!token.empty()) {
                                          this->tok(token);
                                          token.clear();
                                    }
                                    this->tok(i);
                              } else {
                                    token += i;
                              }
                              break;
                        }
                        case '\"':
                        case '\'': {
                              if (i == in_string) {
                                    in_string = '\0';
                              } else if (!in_string) {
                                    if (!token.empty()) {
                                          this->tok(token);
                                          token.clear();
                                    }
                                    in_string = i;
                                    this->tok(i);
                                    break;
                              }
                              if (in_string) {
                                    token += i;
                              }
                              if (!token.empty() && !in_string) {
                                    this->tok(token);
                                    token.clear();
                                    this->tok(i);
                              }
                              break;
                        }
                        case '$':
                        case '\n':
                        case ' ': {
                              if (in_string) {
                                    token += i;
                              } else {
                                    if (!token.empty()) {
                                          this->tok(token);
                                    }
                                    token.clear();
                                    if (i == '\n' && next != '\n') {
                                          this->tok("\\n");
                                    } else if (i == ' ' && next != ' ') {
                                          this->tok(' ');
                                    }
                              }
                              break;
                        }
                        case '\a':
                        case '\b':
                        case '\f':
                        case '\r':
                        case '\v':
                        case '\xdd': {
                              break;
                        }
                        case '\t': {
                              this->tok(' ');
                              break;
                        }
                        default: {
                              token += i;
                              break;
                        }
                  }
            }
            escape = false;
            prev = i;
      }
      return;
}

void fizzle::tokens::tokenize(const std::string &code, const tokenizer_langs l) {
      this->lang = l;
      this->code = code;
      this->raw.clear();
      switch (this->lang) {
            case tokenizer_langs::lua: {
                  this->parser("--", "[[", "]]");
                  break;
            }
            case tokenizer_langs::lua_kosmic: {
                  this->parser("--", "[[", "]]");
                  break;
            }
            case tokenizer_langs::c:
            case tokenizer_langs::cpp:
            case tokenizer_langs::java:
            case tokenizer_langs::javascript: {
                  this->parser("//", "/*", "*/");
                  break;
            }
            case tokenizer_langs::x86asm:
            case tokenizer_langs::x64asm: {
                  this->parser(";", "", "");
                  break;
            }
            case tokenizer_langs::python: {
                  this->parser("#", "```", "```");
                  break;
            }
            default: {
                  break;
            }
      }

      return;
}