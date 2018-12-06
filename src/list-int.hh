#pragma once


struct ListEmpty
{
};

template <int Head, class Tail>
struct List
{
  static constexpr int data = Head;
  using tail = Tail;
};

template <class L>
struct ListSum
{
  static constexpr int val = L::data + ListSum<typename L::tail>::val;
};

template <>
struct ListSum<ListEmpty>
{
  static constexpr int val = 0;
};


template <class L>
struct ListFront
{
  static constexpr int val = L::data;
};

template <class L>
struct ListBack
{
  static constexpr int val = ListBack<typename L::tail>::val;
};

template <int Head>
struct ListBack<List<Head, ListEmpty>>
{
  static constexpr int val = Head;
};


template <class L>
struct ListPopFront
{
  using type_t = typename L::tail;
};


template <class L>
struct ListPopBack
{
  using type_t = List<L::data, typename ListPopBack<typename L::tail>::type_t>;
};

template <int Head>
struct ListPopBack<List<Head, ListEmpty>>
{
  using type_t = ListEmpty;
};

template <class L>
void list_print()
{
  std::cout << L::data << " ";
  list_print<typename L::tail>();
}

template<>
void list_print<ListEmpty>()
{}
