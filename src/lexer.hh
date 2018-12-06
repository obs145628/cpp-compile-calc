#pragma once

#include "list.hh"
#include "compile-str.hh"

static constexpr int TYPE_EOF = 0;
static constexpr int OP_ADD = 1;
static constexpr int OP_SUB = 2;
static constexpr int OP_MUL = 3;
static constexpr int OP_DIV = 4;
static constexpr int CONST_INT = 101;
static constexpr int SYM_LP = 201;
static constexpr int SYM_RP = 202;


template<char C>
struct is_digit_char
{
  static constexpr bool val = (C >= '0' && C <= '9');
};

template <class T, int Val = 0, class _ = void>
struct IntParser
{
  static constexpr int val = Val;
  using others = T;
};

template <char C, int Val, class Tail>
struct IntParser<List<CharItem<C>, Tail>, Val,
		 std::enable_if_t<is_digit_char<C>::val>>
{
  static constexpr int _digit = C - '0';
  using _next = IntParser<Tail, Val * 10 + _digit>;
  
  static constexpr int val = _next::val;
  using others = typename _next::others;
};

template <int Type, int Val>
struct Token
{
  static constexpr int type = Type;
  static constexpr int val = Val;
};

template <int Type, int Val>
struct TypePrinter<Token<Type, Val>>
{
  static void exec(std::ostream& os)
  {
    os << "{ type = " << Type << ", val = " << Val << "}";
  }
};

template <class T, class _ = void>
struct GetToken
{};

template<>
struct GetToken<ListEmpty>
{
  using token = Token<TYPE_EOF, 0>;
};

template <class Tail>
struct GetToken<List<CharItem<'+'>, Tail>>
{
  using token = Token<OP_ADD, 0>;
  using others = Tail;
};
template <class Tail>
struct GetToken<List<CharItem<'-'>, Tail>>
{
  using token = Token<OP_SUB, 0>;
  using others = Tail;
};
template <class Tail>
struct GetToken<List<CharItem<'*'>, Tail>>
{
  using token = Token<OP_MUL, 0>;
  using others = Tail;
};
template <class Tail>
struct GetToken<List<CharItem<'/'>, Tail>>
{
  using token = Token<OP_DIV, 0>;
  using others = Tail;
};

template <char C, class Tail>
struct GetToken<List<CharItem<C>, Tail>,
		std::enable_if_t<is_digit_char<C>::val>>
{
  using _parser = IntParser<List<CharItem<C>, Tail>>;
  using token = Token<CONST_INT, _parser::val>;
  using others = typename _parser::others;
};

template <class Tail>
struct GetToken<List<CharItem<'('>, Tail>>
{
  using token = Token<SYM_LP, 0>;
  using others = Tail;
};
template <class Tail>
struct GetToken<List<CharItem<')'>, Tail>>
{
  using token = Token<SYM_RP, 0>;
  using others = Tail;
};


template <class T>
struct Lexer
{
  using _get_token = GetToken<T>;
  using _first_token = typename _get_token::token;
  using _other_chars = typename _get_token::others;

  using tokens = List<_first_token, typename Lexer<_other_chars>::tokens>; 
};

template<>
struct Lexer<ListEmpty>
{
  using tokens = List<Token<TYPE_EOF, 0>, ListEmpty>;
};


