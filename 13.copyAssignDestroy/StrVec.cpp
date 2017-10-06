//
//Class That Manage Dynamic Memory
//
//
#include "stdafx.h"
//#include<iostream>
//#include<vector>
//#include<string>
//#include<memory>
//
//using namespace std;
//
//class StrVec {
//	StrVec() : elements(nullptr), cap(nullptr),
//		first_free(nullptr) { }
//	StrVec(const StrVec &);
//	StrVec & operator=(const StrVec &);
//	~StrVec();
//	void push_back(const StrVec &);
//	size_t size() { return cap - elements; }
//	size_t capacity() { return cap - first_free; }
//	string * begin() const { return elements; }
//	string * end() const { return first_free; }
//private:
//	allocator<string> alloc;
//	void chk_n_copy() {
//		if (size() == capacity()) {
//			reallocate();
//		}
//	}
//	void free();
//	pair<string *, string*> alloc_n_copy(const string *, const string*);
//	void reallocate();
//	string *elements;
//	string *cap;
//	string *first_free;
//};
//
//pair<string *, string *> StrVec::alloc_n_copy(const string *b, const string *e) {
//	string *data = alloc.allocate(e - b);
//	return {data, uninitialized_copy(b, e, data)};
//}
//
//StrVec::StrVec(const StrVec &s) {
//	pair<string *, string *> data = alloc_n_copy(s.begin(), s.end());
//	elements = data.first;
//	first_free = cap = data.second;
//}
//
//StrVec &StrVec::operator=(const StrVec &s) {
//	pair<string *, string *> data = alloc_n_copy(s.begin(), s.end());
//	elements = data.first;
//	first_free = cap = data.second;
//}
//void StrVec::free() {
//	if (elements) {
//		for (string *p = first_free; p != elements; ) {
//			alloc.destroy(--p);
//		}
//		alloc.deallocate(elements, cap - elements);
//	}
//}
//
//StrVec::~StrVec() { free(); }
//
//void StrVec::push_back(const StrVec &s) {
//	chk_n_copy();
//	alloc.construct(first_free++, s);
//}
//
//void StrVec::reallocate() {
//	size_t newCapacity = size() ? 2 * size() : 1;
//	string * newData = alloc.allocate(newCapacity);
//	string *dest = newData;
//	string *elem = elements;
//	for (size_t i = 0; i != size(); i++) {
//		alloc.construct(dest++, std::move(*elem++));
//	}
//	free();
//	elements = newData;
//	first_free = dest;
//	cap = elements + newCapacity;
//}
int main(void) {

	return 0;
}