#include <iostream>
#include "compile-str.hh"
#include "lexer.hh"
#include "parser.hh"

//#define CALC_STR "19"
//#define CALC_STR #include "input.txt"

int main()
{

  constexpr const char input_str[] =
  {
     #include "input.txt"
  };
  
  using s = STR_LIST(input_str);
  
  using tokens = Lexer<s>::tokens;
  using ast = Parser<tokens>::ast;
  std::cout << ast::eval() << std::endl;
  return 0;
}
