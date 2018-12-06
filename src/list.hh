#pragma once


struct ListEmpty
{
};

template <class DataT, class Tail>
struct List
{
  using Data = DataT;
  using tail = Tail;
};


template <class L>
struct ListFront
{
  using type_t = typename L::Data;
};

template <class L>
struct ListBack
{
  using type_t = typename ListBack<typename L::tail>::type_t;
};

template <class DataT>
struct ListBack<List<DataT, ListEmpty>>
{
  using type_t = DataT;
};


template <class L>
struct ListPopFront
{
  using type_t = typename L::tail;
};


template <class L>
struct ListPopBack
{
  using type_t = List<typename L::Data, typename ListPopBack<typename L::tail>::type_t>;
};

template <class DataT>
struct ListPopBack<List<DataT, ListEmpty>>
{
  using type_t = ListEmpty;
};

template <class L, class DataT>
struct ListPushFront
{
  using type_t = List<DataT, L>;
};

template <class L, class DataT>
struct ListPushBack
{
  using type_t = List<typename L::Data, typename ListPushBack<typename L::tail, DataT>::type_t>;
};

template <class DataT>
struct ListPushBack<ListEmpty, DataT>
{
  using type_t = List<DataT, ListEmpty>;
};


template <class T>
struct TypePrinter
{
  static void exec(std::ostream& os)
  {
    os << T::val;
  }
};

template <class L>
void list_print(std::ostream& os, const char* sep = " ")
{
  TypePrinter<typename L::Data>::exec(os);
  os << sep;
  list_print<typename L::tail>(os, sep);
}

template<>
void list_print<ListEmpty>(std::ostream&, const char*)
{}


