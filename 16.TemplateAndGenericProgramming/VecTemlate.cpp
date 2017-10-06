#include "stdafx.h"
#include <iostream>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <initializer_list>

using namespace std;

template<typename T> class Vec;

template<typename T> bool operator==(const Vec<T>&, const Vec<T>&);
template<typename T> bool operator!=(const Vec<T>&, const Vec<T>&);
template<typename T> bool operator<=(const Vec<T>&, const Vec<T>&);
template<typename T> bool operator>=(const Vec<T>&, const Vec<T>&);
template<typename T> bool operator< (const Vec<T>&, const Vec<T>&);
template<typename T> bool operator> (const Vec<T>&, const Vec<T>&);
template<typename T> ostream& operator<<(ostream&, Vec<T>&);

template<typename T>
class Vec {
  friend bool operator==<T>(const Vec<T> &lhs, const Vec<T> &rhs);
  friend bool operator!=<T>(const Vec<T> &lhs, const Vec<T> &rhs);
  friend bool operator<=<T>(const Vec<T> &lhs, const Vec<T> &rhs);
  friend bool operator>=<T>(const Vec<T> &lhs, const Vec<T> &rhs);
  friend bool operator< <T>(const Vec<T> &lhs, const Vec<T> &rhs);
  friend bool operator> <T>(const Vec<T> &lhs, const Vec<T> &rhs);
  friend ostream& operator<<<T>(ostream&, Vec<T>&);
public:
  Vec(): elements(nullptr), first_free(nullptr), cap(nullptr) {}
  Vec(initializer_list<T>);
  Vec(const Vec<T>&);
  Vec& operator=(const Vec<T>&);
  Vec(Vec<T>&&) noexcept;
  Vec& operator=(Vec<T>&&) noexcept;
  ~Vec() { free(); }

  void push_back(const T&);
  size_t size() const { return first_free - elements; }
  size_t capacity() const { return cap - elements; }
  T* begin() const { return elements; }
  T* end() const { return first_free; }
  T& at(size_t n) { return *(elements + n); }
  const T& at(size_t n) const { return *(elements + n); }
  T& operator[](size_t n) { 
    if (n >= first_free - elements) throw out_of_range("out of range.");
    return elements[n];
  }
  const T& operator[](size_t n) const { 
    if (n >= first_free - elements) throw out_of_range("out of range.");
    return elemetns[n]; 
  }

  void reserve(size_t);
  void resize(size_t);
  void resize(size_t, const T&);

private:
  pair<T*, T*> alloc_n_copy(const T*, const T*);
  void free();
  void alloc_n_move(size_t);
  void chk_n_copy() {
    if (size() == capacity()) reallocate();
  }
  void range_initialize(const T*, const T*);
  void reallocate();

  T *elements;
  T * first_free;
  T *cap;
  std::allocator<T> alloc;
};

template<typename T> bool operator==(const Vec<T> &lhs, const Vec<T> &rhs) {
  return lhs.size() == rhs.size() && std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<typename T> bool operator!=(const Vec<T> &lhs, Vec<T> &rhs) {
  return !(lhs == rhs);
}

template<typename T> bool operator<(const Vec<T> &lhs, const Vec<T> &rhs) {
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<typename T> bool operator<=(const Vec<T> &lhs, const Vec<T> &rhs) {
  return !(rhs < lhs);
}

template<typename T> bool operator>(const Vec<T> &lhs, const Vec<T> &rhs) {
  return rhs < lhs;
}

template<typename T> bool operator>=(const Vec<T> &lhs, const Vec<T> &rhs) {
  return !(lhs < rhs);
}

template<typename T> ostream& operator<<(ostream &os, Vec<T> &v) {
  // iostream can not copy
  for_each(v.begin(), v.end(), [&os] (T &vi) { os << vi << ", "; });

  /*for (auto i = v.begin(); i != v.end(); i++) {
    os << *i << ", ";
  }*/
  os << endl;
  return os;
}

/*******************************   function member  ****************************************/
template<typename T> Vec<T>::Vec(initializer_list<T> il) {
  range_initialize(il.begin(), il.end());
}

template<typename T> Vec<T>::Vec(const Vec<T> &d) {
  range_initialze(d.begin(), d.end());
}

template<typename T> Vec<T>::Vec(Vec<T> &&d)noexcept :
  elements(d.elements), first_free(d.first_free), cap(d.cap){
  elements = firsr_free = cap = nullptr;
}

template<typename T> Vec<T>& Vec<T>::operator=(Vec<T> &&d) noexcept {
  if (*this != d) {
    free();
    elements = d.elements;
    firs_free = d.firsr_free;
    cap = d.cap;
    elements = first_free = cap = nullptr;
  }
  return *this;
}

template<typename T> void Vec<T>::reserve(size_t new_cap) {
  if (new_cap < capacity()) return;
  alloc_n_move(new_cap);
}

template<typename T> void Vec<T>::resize(size_t count) {
  resize(count, T());
}

template<typename T> void Vec<T>::resize(size_t count, const T &p) {
  if (count > size()) {
    if (count > capacity()) reserve(count * 2);
    for (auto i = size(); i < count; i++)
      alloc.construct(first_free++, p);
  } else if (count < capacity()) {
    while (first_free != elements + count) alloc.destroy(--first_free);
  }
}

template<typename T> Vec<T>& Vec<T>::operator=(const Vec<T> &d) {
  auto data = alloc_n_copy(d.begin, d.end());
  free();
  elements = data.first;
  first_free = data.second;
  return *this;
}

template<typename T> void Vec<T>::push_back(const T& d) {
  chk_n_copy();
  alloc.construct(first_free++, d);
}

template<typename T> pair<T*, T*> Vec<T>::alloc_n_copy(const T *b, const T *e) {
  auto data = alloc.allocate(e - b);
  return {data, std::uninitialized_copy(b, e, data)};
}

template<typename T> void Vec<T>::alloc_n_move(size_t new_cap) {
  auto new_data = alloc.allocate(new_cap);
  auto data = new_data;
  auto elem = elements;
  for (; elem != first_free;) {
    alloc.construct(data++, std::move(*elem++));
  }
  free();
  elements = new_data;
  first_free = data;
  cap = elements + new_cap;
}

template<typename T> void Vec<T>::free() {
  if (elements) {
    for_each(elements, first_free, [this] (T &rhs) { alloc.destroy(&rhs); });
    alloc.deallocate(elements, cap - elements);
  }
}

template<typename T> void Vec<T>::range_initialize(const T *b, const T *e) {
  auto data = alloc_n_copy(b, e);
  elements = data.first;
  first_free = cap = data.second;
}

template<typename T> void Vec<T>::reallocate() {
  auto new_cap = size() ? 2 * size() : 1;
  alloc_n_move(new_cap);
}

int main(void) {
  Vec<int> vi({1, 2, 3, 4});
  vi.push_back(5);

  cout << "begin_: " << vi.begin() << endl;
  vi.reserve(10);
  cout << "begin_reserve: " << vi.begin() << endl;
  vi.resize(3);
  cout << "begin_resize: " << vi.begin() << endl;

  std::cout << vi;
  std::cout << vi[3] << std::endl;
  return 0;
}

