#pragma once

#include "lexer.hh"

template <class Token>
struct IntOp
{
  using token = Token;

  static int eval()
  {
    return token::val;
  }
};
  
template <class Left, class Right>
struct AddOp
{
  using left = Left;
  using right = Right;

  static int eval()
  {
    return left::eval() + right::eval();
  }
};

template <class Left, class Right>
struct SubOp
{
  using left = Left;
  using right = Right;

  static int eval()
  {
    return left::eval() - right::eval();
  }
};

template <class Left, class Right>
struct MulOp
{
  using left = Left;
  using right = Right;

  static int eval()
  {
    return left::eval() * right::eval();
  }
};

template <class Left, class Right>
struct DivOp
{
  using left = Left;
  using right = Right;

  static int eval()
  {
    return left::eval() / right::eval();
  }
};

template <class Left, class Right, class Token>
struct BinOpBuilder
{};

template <class Left, class Right>
struct BinOpBuilder<Left, Right, Token<OP_ADD, 0>>
{
  using ast = AddOp<Left, Right>;
};

template <class Left, class Right>
struct BinOpBuilder<Left, Right, Token<OP_SUB, 0>>
{
  using ast = SubOp<Left, Right>;
};

template <class Left, class Right>
struct BinOpBuilder<Left, Right, Token<OP_MUL, 0>>
{
  using ast = MulOp<Left, Right>;
};

template <class Left, class Right>
struct BinOpBuilder<Left, Right, Token<OP_DIV, 0>>
{
  using ast = DivOp<Left, Right>;
};


template <class Tokens>
struct RuleInt
{};

template <int Val, class Tail>
struct RuleInt<List<Token<CONST_INT, Val>, Tail>>
{
  using ast = IntOp<Token<CONST_INT, Val>>;
  using others = Tail;
};

template <class Tokens>
struct RuleEof{};

template <class Tail>
struct RuleEof<List<Token<TYPE_EOF, 0>, Tail>>
{
  using others = List<Token<TYPE_EOF, 0>, Tail>;
};

template <class Tokens, int Type>
struct EatToken{};

template <class Tail, int Type>
struct EatToken<List<Token<Type, 0>, Tail>, Type>
{
  using token = Token<Type, 0>;
  using others = Tail;
};


template <class Tokens>
struct RuleExpParen;

template <class Tokens>
struct RuleExpP0
{
  using _r1 = RuleInt<Tokens>;
  using ast = typename _r1::ast;
  using others = typename _r1::others;
};

template <class Tail>
struct RuleExpP0<List<Token<SYM_LP, 0>, Tail>>
{
  using _r1 = RuleExpParen<List<Token<SYM_LP, 0>, Tail>>;
  using ast = typename _r1::ast;
  using others = typename _r1::others;
};

template <class Tokens, class Ops, class _ = void, int... Types>
struct OptionalOp
{};

template <class Tokens, class Ops>
struct OptionalOp<Tokens, Ops>
{
  static constexpr bool found = false;
  using token = Token<TYPE_EOF, 0>;
  using others = Tokens;
  using ops = Ops;
};

template <class Tokens, class Ops, int Type, int... Others>
struct OptionalOp<Tokens, Ops,
		  std::enable_if_t<Tokens::Data::type == Type>,
		  Type, Others...>
{
  static constexpr bool found = true;
  using token = typename Tokens::Data;
  using others = typename Tokens::tail;
  using ops = typename ListPushFront<Ops, typename Tokens::Data>::type_t;
};

template <class Tokens, class Ops, int Type, int... Others>
struct OptionalOp<Tokens, Ops,
		  std::enable_if_t<Tokens::Data::type != Type>,
		  Type, Others...>
{
  using _rec = OptionalOp<Tokens, Ops, void, Others...>;
  static constexpr bool found = _rec::found;
  using token = typename _rec::token;
  using others = typename _rec::others;
  using ops = typename _rec::ops;
};

template <class Tokens, class Vals, class Ops, class NextRule, bool Stop, int Type1, int Type2>
struct ExpFill
{
  using vals = Vals;
  using ops = Ops;
  using others = Tokens;
};

template <class Tokens, class Vals, class Ops, class NextRule, int Type1, int Type2>
struct ExpFill<Tokens, Vals, Ops, NextRule, false, Type1, Type2>
{
  //using _r1 = RuleExpP0<Tokens>;
  using _r1 = typename NextRule::template rule<Tokens>;
  using _r2 = OptionalOp<typename _r1::others, Ops, void, Type1, Type2>;

  using _vals = typename ListPushFront<Vals, typename _r1::ast>::type_t;
  using _ops = typename _r2::ops;
  using _others = typename _r2::others;

  using _rec = ExpFill<_others, _vals, _ops, NextRule, !_r2::found, Type1, Type2>;
  using vals = typename _rec::vals;
  using ops = typename _rec::ops;
  using others = typename _rec::others;
};

template <class Vals, class Ops>
struct ExpUnfill
{
  using _v1 = typename ListBack<Vals>::type_t;
  using _vals1 = typename ListPopBack<Vals>::type_t;
  using _v2 = typename ListBack<_vals1>::type_t;
  using _vals2 = typename ListPopBack<_vals1>::type_t;

  using _op = typename ListBack<Ops>::type_t;
  using _ops1 = typename ListPopBack<Ops>::type_t;

  using _node = typename BinOpBuilder<_v1, _v2, _op>::ast;
  using _vals3 = typename ListPushBack<_vals2, _node>::type_t;

  using ast = typename ExpUnfill<_vals3, _ops1>::ast;
};

template <class Vals>
struct ExpUnfill<Vals, ListEmpty>
{
  using ast = typename Vals::Data;
};

template <class Tokens>
struct RuleExpP1
{

  struct NextRule
  {
    template <class T>
    using rule = RuleExpP0<T>;
  };

  using _fill = ExpFill<Tokens, ListEmpty, ListEmpty, NextRule, false, OP_MUL, OP_DIV>;
  using _unfill = ExpUnfill<typename _fill::vals, typename _fill::ops>;

  using ast = typename _unfill::ast;
  using others = typename _fill::others;
};

template <class Tokens>
struct RuleExpP2
{

  struct NextRule
  {
    template <class T>
    using rule = RuleExpP1<T>;
  };

  using _fill = ExpFill<Tokens, ListEmpty, ListEmpty, NextRule, false, OP_ADD, OP_SUB>;
  using _unfill = ExpUnfill<typename _fill::vals, typename _fill::ops>;

  using ast = typename _unfill::ast;
  using others = typename _fill::others;
};

template <class Tokens>
struct RuleExp
{
  using _r1 = RuleExpP2<Tokens>;  
  using ast = typename _r1::ast;
  using others = typename _r1::others;
};

template <class Tokens>
struct RuleExpParen
{
  using _r1 = EatToken<Tokens, SYM_LP>;
  using _r2 = RuleExp<typename _r1::others>;
  using _r3 = EatToken<typename _r2::others, SYM_RP>;
  using ast = typename _r2::ast;
  using others = typename _r3::others;
};


template <class Tokens>
struct RuleMain
{
  using _r1 = RuleExp<Tokens>;
  using _r2 = RuleEof<typename _r1::others>;
  
  using ast = typename _r1::ast;
  using others = typename _r2::others;
};


template <class Tokens>
struct Parser
{
  using ast = typename RuleMain<Tokens>::ast;
};
