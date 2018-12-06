#include "eval-runtime.hh"
#include <cassert>
#include <stdexcept>
#include <vector>

#include <iostream>

namespace
{

  static constexpr int TYPE_EOF = 0;
  static constexpr int OP_ADD = 1;
  static constexpr int OP_SUB = 2;
  static constexpr int OP_MUL = 3;
  static constexpr int OP_DIV = 4;
  static constexpr int CONST_INT = 101;
  
  const char* g_it;
  int g_type;
  int g_val;

  void read_next()
  {
    char c = *g_it;

    if (c == '\0')
    {
      g_type = TYPE_EOF;
    }
    
    else if (c == '+')
    {
      ++g_it;
      g_type = OP_ADD;
    }

    else if (c == '-')
    {
      ++g_it;
      g_type = OP_SUB;
    }

    else if (c == '*')
    {
      ++g_it;
      g_type = OP_MUL;
    }

    else if (c == '/')
    {
      ++g_it;
      g_type = OP_DIV;
    }

    else if (c >= '0' && c <= '9')
    {
      int val = 0;
      while (true)
      {
	char c = *g_it;
	if (c < '0' || c >= '9')
	  break;
	++g_it;
	val = 10 * val + (c - '0');
      }
      g_type = CONST_INT;
      g_val = val;
    }

    else
    {
      throw std::runtime_error {"Invalid char: " + std::to_string(c) + "."};
    }
    
  }

  void peek_next()
  {
    const char* tmp = g_it;
    read_next();
    g_it = tmp;
  }

  int r_const()
  {
    read_next();
    if (g_type != CONST_INT)
      throw std::runtime_error {"Constant expected"};
    return g_val;
  }

  void r_eof()
  {
    read_next();
    if (g_type != TYPE_EOF)
      throw std::runtime_error {"End-Of-File expected"};
  }

  /*
  int expr()
  {
    int res = 0;

    while (true)
    {
	
    }

    return res;
    }*/

  int r_exp_p0()
  {
    return r_const();
  }

  void r_exp_p1_fill(std::vector<int>& vals, std::vector<int>& ops)
  {
    int val = r_exp_p0();
    vals.insert(vals.begin(), val);
    peek_next();
    int op = g_type;
    if (op != OP_MUL && op != OP_DIV)
      return;

    read_next();
    ops.insert(ops.begin(),  op);
    r_exp_p1_fill(vals, ops);
  }

  void r_exp_p1_eval(std::vector<int>& vals, std::vector<int>& ops)
  {
    if (vals.size() == 1)
      return;

    assert(!ops.empty());
    assert(vals.size() > 1);
    int v1 = vals.back();
    vals.erase(vals.end()-1);
    int v2 = vals.back();
    vals.erase(vals.end()-1);
    int op = ops.back();
    ops.erase(ops.end()-1);

    if (op == OP_MUL)
      vals.push_back(v1 * v2);
    else if (op == OP_DIV)
      vals.push_back(v1 / v2);
    else
      throw std::runtime_error {"Unreachable"};

    r_exp_p1_eval(vals, ops);
  }

  int r_exp_p1()
  {
    std::vector<int> vals;
    std::vector<int> ops;
    r_exp_p1_fill(vals, ops);
    r_exp_p1_eval(vals, ops);    
    assert(vals.size() == 1);
    assert(ops.size() == 0);
    return vals.front();
  }

  void r_exp_p2_fill(std::vector<int>& vals, std::vector<int>& ops)
  {
    int val = r_exp_p1();
    vals.insert(vals.begin(), val);
    peek_next();
    int op = g_type;
    if (op != OP_ADD && op != OP_SUB)
      return;

    read_next();
    ops.insert(ops.begin(),  op);
    r_exp_p2_fill(vals, ops);
  }

  void r_exp_p2_eval(std::vector<int>& vals, std::vector<int>& ops)
  {
    if (vals.size() == 1)
      return;

    assert(!ops.empty());
    assert(vals.size() > 1);
    int v1 = vals.back();
    vals.erase(vals.end()-1);
    int v2 = vals.back();
    vals.erase(vals.end()-1);
    int op = ops.back();
    ops.erase(ops.end()-1);

    if (op == OP_ADD)
      vals.push_back(v1 + v2);
    else if (op == OP_SUB)
      vals.push_back(v1 - v2);
    else
      throw std::runtime_error {"Unreachable"};

    r_exp_p2_eval(vals, ops);
  }

  int r_exp_p2()
  {
    std::vector<int> vals;
    std::vector<int> ops;
    r_exp_p2_fill(vals, ops);
    r_exp_p2_eval(vals, ops);    
    assert(vals.size() == 1);
    assert(ops.size() == 0);
    return vals.front();
  }

  int r_exp()
  {
    int res = r_exp_p2();
    r_eof();
    return res;
  }
  
  
}


int eval_runtime(const char* s)
{
  g_it = s;
  g_type = TYPE_EOF;
  g_val = 0;
  return r_exp();
}
