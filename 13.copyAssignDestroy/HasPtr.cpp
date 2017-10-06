#include"stdafx.h"
#include<iostream>
#include<string>

using namespace std;

/*
*
*	class act like value
*
*/

//class HasPtr{
//public:
//	HasPtr(string s = string()) : ps(new string(s)), i(0) {}
//	HasPtr(const HasPtr &hp) : ps(new string(*hp.ps)), i(hp.i) {}
//	HasPtr & operator= (const HasPtr &hp);
//	~HasPtr() {
//		delete ps;
//	}
//
//	string *ps;
//	int i;
//};
//
//HasPtr & HasPtr::operator=(const HasPtr &hp) {
//	string *p = new string(*hp.ps);
//	delete ps;
//	ps = hp.p;
//	i = hp.i;
//	return *this;
//}

/*
*
*	class act like pointer
*
*/

class HasPtr {
public:
	HasPtr(string str = string()):
		ps(new string(str)), i(0), use(new size_t()) { ++*use;}
	HasPtr(const HasPtr &p): ps(p.ps), i(p.i), use(p.use) { ++*use; }
	HasPtr & operator=(const HasPtr &p);
	~HasPtr();

	string *ps;
	int i;
	size_t *use;
};

HasPtr & HasPtr::operator=(const HasPtr &p) {
	++*use;
	if (--*use == 0) {
		delete ps;
		delete use;
	}
	ps = p.ps;
	use = p.use;
	i = p.i;
	return *this;
}

HasPtr::~HasPtr() {
	if (--*use == 0) {
		delete ps;
		delete use;
		cout << "~HasPtr" << endl;
	}
}

//int main(void) {
//	// class act like value
//	/*HasPtr p;
//	cout << "p ps: " << p.ps << endl;
//	HasPtr m;
//	cout << "m ps: " << m.ps << endl;
//	m = p;
//	cout << "m new ps: " << m.ps << endl;*/
//
//
//	// class behavior like pointer
//	{
//		HasPtr p("Jemmy");
//		cout << "P use : " << *p.use << endl;
//		HasPtr m(p);
//		cout << *m.use << endl;
//	}
//
//
//	return 0;
//}