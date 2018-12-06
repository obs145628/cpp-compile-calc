#pragma once

#include "list.hh"

template <char C>
struct CharItem
{
  static constexpr char val = C;
};

template <char... C>
struct SeqToList
{
  using list = ListEmpty;
};

template <char Head, char... Tail>
struct SeqToList<Head, Tail...>
{
  using list = List<CharItem<Head>, typename SeqToList<Tail...>::list>;
};

template <char... Tail>
struct SeqToList<0, Tail...>
{
  using list = ListEmpty;
};

template <std::size_t N>
constexpr char str_char_at(char const(&str)[N], std::size_t i)
{
  return i < N ? str[i] : '\0';
}

#define SEQ_STR4(s, i) str_char_at(s,i),str_char_at(s, i+1), str_char_at(s,i+2),str_char_at(s,i+3)
#define SEQ_STR16(s, i) SEQ_STR4(s,i),SEQ_STR4(s,i+4),SEQ_STR4(s,i+8),SEQ_STR4(s,i+12)
#define SEQ_STR64(s, i) SEQ_STR16(s,i),SEQ_STR16(s,i+16),SEQ_STR16(s,i+32),SEQ_STR16(s,i+48)
#define SEQ_STR256(s, i) SEQ_STR64(s,i),SEQ_STR64(s,i+64),SEQ_STR64(s,i+128),SEQ_STR64(s,i+192)
#define SEQ_STR1024(s, i) SEQ_STR256(s,i),SEQ_STR256(s,i+256),SEQ_STR256(s,i+512),SEQ_STR256(s,i+768)
#define STR_LIST(s) SeqToList<SEQ_STR1024(s, 0)>::list
