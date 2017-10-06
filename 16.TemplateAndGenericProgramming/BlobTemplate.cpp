#include "stdafx.h"

#include<iostream>
#include<string>
using std::cout;
using std::endl;

#include <vector>
using std::vector;

#include <initializer_list>
using std::initializer_list;

#include <memory>
using std::make_shared;
using std::shared_ptr;
using std::weak_ptr;

#include <algorithm>
#include <stdexcept>
using std::runtime_error;
using std::out_of_range;

template<typename T> class Blob;
template<typename T> class BlobPtr;
template<typename T> class ConstBlobPtr;
////////////////////////////////////////////////////////////////////////////////////////////
//                                    Blob
////////////////////////////////////////////////////////////////////////////////////////////

template<typename T> bool operator==(const Blob<T>&, const Blob<T>&);
template<typename T> bool operator!=(const Blob<T>&, const Blob<T>&);
template<typename T> bool operator> (const Blob<T>&, const Blob<T>&);
template<typename T> bool operator>=(const Blob<T>&, const Blob<T>&);
template<typename T> bool operator< (const Blob<T>&, const Blob<T>&);
template<typename T> bool operator<=(const Blob<T>&, const Blob<T>&);

template<typename T>
class Blob {
  friend class BlobPtr<T>;
  friend class ConstBlobPtr<T>;
  friend bool operator==<T>(const Blob<T>&, const Blob<T>&);
  friend bool operator!=<T>(const Blob<T>&, const Blob<T>&);
  friend bool operator< <T>(const Blob<T>&, const Blob<T>&);
  friend bool operator<=<T>(const Blob<T>&, const Blob<T>&);
  friend bool operator> <T>(const Blob<T>&, const Blob<T>&);
  friend bool operator>=<T>(const Blob<T>&, const Blob<T>&);
public:
  typedef typename vector<T>::size_type size_type;

  Blob(): data(make_shared<vector<T>>()){}
  Blob(initializer_list<T> il): data(make_shared<vector<T>>(il)) {}

  Blob(const Blob<T> &lhs): data(make_shared<vector<T>>(*lhs.data)){}
  Blob& operator=(const Blob<T> &lhs);

  Blob(Blob<T> &&lhs) noexcept : data(std::move(*lhs.data)){}
  Blob& operator=(Blob<T> &&lhs) noexcept;

  BlobPtr<T> begin();
  BlobPtr<T> end();
  ConstBlobPtr<T> cbegin() const;
  ConstBlobPtr<T> cend() const;

  T& operator[](size_t n);
  const T& operator[] (size_t n) const;

  void push_back(const T &lhs) { data->push_back(lhs); }
  void push_back(T &&lhs) { data->push_back(std::move(lhs)); }

  size_type size() const { return data->size(); }
  bool empty() { return data->empty(); }

  T& front();
  T& back();
  const T& front() const;
  const T& back() const;

  void pop_back();

private:
  void check(size_type, const T&) const;
  shared_ptr<vector<T>> data;
};
/*******************************  friend operations  ****************************************/
template<typename T> bool operator==(const Blob<T> &lhs, const Blob<T> &rhs) {
  return *lhs.data == *rhs.data;
}

template<typename T> bool operator!=(const Blob<T> &lsh, const Blob<T> &rhs) {
  ruturn !(rhs == lhs);
}

template<typename T> bool operator< (const Blob<T> &lhs, const Blob<T> &rhs) {
  return std::lexicographical_compare(lhs.data->begin(), lhs.data->end(),
                                      rhs.data->begin(), rhs.data->end());
}

template<typename T> bool operator<=(const Blob<T> &lhs, const Blob<T> &rhs) {
  return !(lhs < rhs);
}

template<typename T> bool operator> (const Blob<T> &lhs, const Blob<T> &rhs) {
  return rhs < lhs;
}

template<typename T> bool operator>=(const Blob<T> &lhs, const Blob<T> &rhs) {
  return !(lhs < rhs);
}

/*******************************   function member  ****************************************/

template<typename T>
Blob<T>& Blob<T>::operator=(const Blob<T> &lhs) {
  data = make_shared<vector<T>>(*lsh.data);
  return *this;
}

template<typename T>
Blob<T>& Blob<T>::operator=(Blob<T> &&lhs) noexcept {
  if (this != lhs) {
    data = std::move(lhs);
    lhs.data = nullptr;
  }
  return *this;
}
template <typename T> BlobPtr<T> Blob<T>::begin() {
  return BlobPtr<T>(*this);
}

template<typename T> inline BlobPtr<T> Blob<T>::end() {
  return BlobPtr<T>(*this, data->size());
}

template<typename T> ConstBlobPtr<T> Blob<T>::cbegin() const {
  return ConstBlobPtr<T>(*this);
}

template<typename T> ConstBlobPtr<T> Blob<T>::cend() const {
  return ConstBlobPtr<T>(*this, data->size());
}

template<typename T> 
inline void Blob<T>::check(size_type i, const T &msg) const {
  if (i > data->size()) throw std::out_of_range(msg);
}

template<typename T> inline T& Blob<T>::operator[](size_t n) {
  check(n, "out of range.");
  return data->at(n);
}
template<typename T> const T& Blob<T>::operator[](size_t n) const{
  check(n, "out of range.");
  return data->at(n);
}

template<typename T> inline T& Blob<T>::front() {
  check(0, "front on empty Blob<T>");
  return data->front();
}

template<typename T> inline const T& Blob<T>::front() const {
  check(0, "const front on empty Blob<T>");
  return data->front();
}

template<typename T> inline T& Blob<T>::back() {
  check(0, "back on empty Blob<T>");
  return data->back();
}

template<typename T> inline const T& Blob<T>::back() const {
  check(0, "const back on empty Blob<T>");
  return data->back();
}

template<typename T> inline void Blob<T>::pop_back() {
  check(0, "pop_back on empty Blob<T>");
  return data->pop_back();
}

////////////////////////////////////////////////////////////////////////////////////////////
//                                    BlobPtr
////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> bool operator==(const BlobPtr<T>&, const BlobPtr<T>&);
template<typename T> bool operator!=(const BlobPtr<T>&, const BlobPtr<T>&);
template<typename T> bool operator< (const BlobPtr<T>&, const BlobPtr<T>&);
template<typename T> bool operator<=(const BlobPtr<T>&, const BlobPtr<T>&);
template<typename T> bool operator> (const BlobPtr<T>&, const BlobPtr<T>&);
template<typename T> bool operator>=(const BlobPtr<T>&, const BlobPtr<T>&);

template<typename T>
class BlobPtr {
  friend bool operator==<T>(const BlobPtr<T>&, const BlobPtr<T>&);
  friend bool operator!=<T>(const BlobPtr<T>&, const BlobPtr<T>&);
  friend bool operator< <T>(const BlobPtr<T>&, const BlobPtr<T>&);
  friend bool operator<=<T>(const BlobPtr<T>&, const BlobPtr<T>&);
  friend bool operator> <T>(const BlobPtr<T>&, const BlobPtr<T>&);
  friend bool operator>=<T>(const BlobPtr<T>&, const BlobPtr<T>&);
public:
  BlobPtr() : curr(0) {}
  BlobPtr(Blob<T> &s, size_t sz = 0) : wptr(s.data), curr(sz) {}
  T& deref() const;
  BlobPtr& operator++();
  BlobPtr& operator--();
  BlobPtr  operator++(int);
  BlobPtr  operator--(int);
  BlobPtr& operator+=(size_t);
  BlobPtr& operator-=(size_t);
  BlobPtr  operator+ (size_t) const;
  BlobPtr  operator- (size_t) const;
  T& operator[](size_t n);
  const T& operator[](size_t n) const;
private:
  shared_ptr<vector<T>> check(size_t, const T&) const;
  std::weak_ptr<vector<T>> wptr;
  size_t curr;
};
template<typename T>
bool operator==(const BlobPtr<T> &lhs, const BlobPtr<T> &rhs) {
  return lhs.curr == rhs.curr;
}

template<typename T>
bool operator!=(const BlobPtr<T> &lhs, const BlobPtr<T> &rhs) {
  return lhs.curr != rhs.curr;
}

template<typename T>
bool operator< (const BlobPtr<T> &lhs, const BlobPtr<T> &rhs) {
  return lhs.curr < rhs.curr;
}

template<typename T>
bool operator<=(const BlobPtr<T> &lhs, const BlobPtr<T> &rhs) {
  return lhs.curr <= rhs.curr;
}

template<typename T>
bool operator> (const BlobPtr<T> &lhs, const BlobPtr<T> &rhs) {
  return lhs.curr > rhs.curr;
}

template<typename T>
bool operator>=(const BlobPtr<T> &lhs, const BlobPtr<T> &rhs) {
  return lhs.curr >= rhs.curr;
}
/*******************************   function member  ****************************************/
template<typename T>
shared_ptr<vector<T>> BlobPtr<T>::check(size_t i, const T& msg) const {
  auto ret = wptr.lock();
  if (!ret) throw runtime_error(msg);
  if (i >= ret->size()) throw out_of_range(msg);
  return ret;
}
template<typename T>
T& BlobPtr<T>::deref() const {
  auto p = check(curr, "dereference past end");
  return (*p)[curr];
}

template<typename T>
inline BlobPtr<T>& BlobPtr<T>::operator++() {
  check(curr, "increment past end of BlobPtr<T>");
  ++curr;
  return *this;
}

template<typename T>
inline BlobPtr<T> BlobPtr<T>::operator++(int) {
  auto ret = *this;
  ++*this;
  return ret;
}

template<typename T>
inline BlobPtr<T>& BlobPtr<T>::operator--() {
  --curr;
  check(-1, "decrement past begin of BlobPtr<T>");
  return *this;
}

template<typename T>
inline BlobPtr<T> BlobPtr<T>::operator--(int) {
  auto ret = *this;
  --*this;
  return ret;
}

template<typename T>
inline BlobPtr<T>& BlobPtr<T>::operator+=(size_t n) {
  curr += n;
  check(curr, "increment past end of BlobPtr<T>");
  return *this;
}

template<typename T>
inline BlobPtr<T>& BlobPtr<T>::operator-=(size_t n) {
  curr -= n;
  check(curr, "decrement past end of BlobPtr<T>");
  return *this;
}

template<typename T>
BlobPtr<T> BlobPtr<T>::operator+(size_t n) const {
  auto ret = *this;
  ret += n;
  return ret;
}

template<typename T>
inline BlobPtr<T> BlobPtr<T>::operator-(size_t n) const {
  auto ret = *this;
  ret -= n;
  return ret;
}

template<typename T>
inline T& BlobPtr<T>::operator[](size_t n) {
  check(n, "dereferecen out of range");
  return (*p)[n];
}

template<typename T>
inline const T& BlobPtr<T>::operator[](size_t n) const {
  check(n, "out of range");
  return (*p)[n];
}
////////////////////////////////////////////////////////////////////////////////////////////
//                                    ConstBlobPtr
////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> bool operator==(const ConstBlobPtr<T>&, const ConstBlobPtr<T>&);
template<typename T> bool operator!=(const ConstBlobPtr<T>&, const ConstBlobPtr<T>&);
template<typename T> bool operator>=(const ConstBlobPtr<T>&, const ConstBlobPtr<T>&);
template<typename T> bool operator<=(const ConstBlobPtr<T>&, const ConstBlobPtr<T>&);
template<typename T> bool operator< (const ConstBlobPtr<T>&, const ConstBlobPtr<T>&);
template<typename T> bool operator> (const ConstBlobPtr<T>&, const ConstBlobPtr<T>&);

template<typename T>
class ConstBlobPtr {
  friend bool operator==<T>(const ConstBlobPtr<T> &lhs, const ConstBlobPtr<T> &rhs);
  friend bool operator!=<T>(const ConstBlobPtr<T> &lhs, const ConstBlobPtr<T> &rhs);
  friend bool operator<=<T>(const ConstBlobPtr<T> &lhs, const ConstBlobPtr<T> &rhs);
  friend bool operator>=<T>(const ConstBlobPtr<T> &lhs, const ConstBlobPtr<T> &rhs);
  friend bool operator< <T>(const ConstBlobPtr<T> &lhs, const ConstBlobPtr<T> &rhs);
  friend bool operator> <T>(const ConstBlobPtr<T> &lhs, const ConstBlobPtr<T> &rhs);
public:
  ConstBlobPtr(): curr(0) {}
  ConstBlobPtr(const Blob<T> &s, size_t sz = 0) : wptr(s.data), curr(sz) {}
  const T& operator*() const;
  const T* operator->() const;
  ConstBlobPtr<T>& operator++();
  ConstBlobPtr<T>& operator--();
  ConstBlobPtr<T>  operator++(int);
  ConstBlobPtr<T>  operator--(int);
  ConstBlobPtr<T>& operator+=(size_t);
  ConstBlobPtr<T>&  operator-=(size_t);
  ConstBlobPtr<T>  operator-(size_t) const;
  ConstBlobPtr<T>  operator+(size_t) const;

  const T& operator[](size_t) const;
private:
  shared_ptr<vector<T>> check(size_t, const T&) const;
  weak_ptr<vector<T>> wptr;
  size_t curr;
};

// friend ship
template<typename T>
bool operator==(const ConstBlobPtr<T> &lhs, const ConstBlobPtr<T> &rhs) {
  return lhs.curr == rhs.curr;
}
template<typename T>
bool operator!=(const ConstBlobPtr<T> &lhs, const ConstBlobPtr<T> &rhs) {
  return lhs.curr != rhs.curr;
}
template<typename T>
bool operator<=(const ConstBlobPtr<T> &lhs, const ConstBlobPtr<T> &rhs) {
  return lhs.curr <= rhs.curr;
}
template<typename T>
bool operator>=(const ConstBlobPtr<T> &lhs, const ConstBlobPtr<T> &rhs) {
  return lhs.curr <= rhs.curr;
}
template<typename T>
bool operator< (const ConstBlobPtr<T> &lhs, const ConstBlobPtr<T> &rhs) {
  return lhs.curr < rhs.curr;
}
template<typename T>
bool operator> (const ConstBlobPtr<T> &lhs, const ConstBlobPtr<T> &rhs) {
  return lhs.curr > rhs.curr;
}
/*******************************   function member  ****************************************/
template<typename T> inline const T& ConstBlobPtr<T>::operator*() const {
  auto p = check(curr, "dereference past end");
  return (*p)[curr];
}
template<typename T> inline const T* ConstBlobPtr<T>::operator->() const {
  return &this->operator*();
}
template<typename T> inline ConstBlobPtr<T>& ConstBlobPtr<T>::operator++() {
  check(curr, "increment past end of ConstBlobPtr<T>");
  ++curr;
  return *this;
}
template<typename T> inline ConstBlobPtr<T> ConstBlobPtr<T>::operator++(int) {
  auto ret = *this;
  ++*this;
  return ret;
}
template<typename T> inline ConstBlobPtr<T>& ConstBlobPtr<T>::operator--() {
  check(curr, "decrement past end of ConstBlobPtr<T>");
  --curr;
  return *this;
}
template<typename T> inline ConstBlobPtr<T>  ConstBlobPtr<T>::operator--(int) {
  auto ret = *this;
  --*this;
  return ret;
}

template<typename T> inline ConstBlobPtr<T>& ConstBlobPtr<T>::operator+=(size_t n) {
  curr += n;
  check(curr, "increment past end of ConstBlobPtr<T>");
  return *this;
}
template<typename T> inline ConstBlobPtr<T>& ConstBlobPtr<T>::operator-=(size_t n) {
  curr -= n;
  check(curr, "decrement past end of ConstBlobPtr<T>");
  return *this;
}
template<typename T> inline ConstBlobPtr<T> ConstBlobPtr<T>::operator+(size_t n) const {
  auto ret = *this;
  ret += n;
  return ret;
}
template<typename T> inline ConstBlobPtr<T> ConstBlobPtr<T>::operator-(size_t n) const {
  auto ret = *this;
  ret -= n;
  return ret;
}
template<typename T> inline const T& ConstBlobPtr<T>::operator[](size_t n) const {
  auto p = check(curr, "dereference out of range");
  return (*p)[curr];
}

template<typename T> 
inline shared_ptr<vector<T>> ConstBlobPtr<T>::check(size_t i, const T& msg) const {
  auto p = wptr.lock();
  if (!p) throw std::runtime_error("unbond Blob<T>Ptr");
  if (i >= p->size()) throw std::out_of_range(msg);
  return p;
}

//int main(void) {
//
//  Blob<std::string> sb1 {"a", "b", "c"};
//  Blob<std::string> sb2 = sb1;
//  Blob<std::string> sb3({"a", "b"});
//
//  sb2[2] = "b";
//  sb2.push_back("Jemmy");
//  sb2.pop_back();
//
//  std::cout << (sb2 < sb3) << std::endl;
//  if (sb1 > sb2) {
//    for (auto iter = sb2.begin(); iter != sb2.end(); ++iter)
//      std::cout << iter.deref() << " ";
//    std::cout << std::endl;
//  }
//
//  ConstBlobPtr<std::string> iter(sb2);
//  std::cout << iter->size() << std::endl;
//
//  return 0;
//}

