#include"stdafx.h"
#include<iostream>
#include<string>

using namespace std;

class Base {
public:
	//~Base() = delete;
	//Base(const Base &) = delete;
	Base(int nu) : num(nu) {}
	Base() {
		
	}
private:
	int num ;

};

class Main {
private :
	int num;
	Base a;
};

//int main(void) {
//	Main m;
//	Main mb = m;
//	return 0;
//}